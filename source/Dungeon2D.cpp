#include "Dungeon2D.hpp"
#include <random>
#include <algorithm>

Dungeon2D::Dungeon2D(int widthTiles, int heightTiles)
    : width(widthTiles), height(heightTiles), root(nullptr) {}

namespace {

// Função auxiliar para criar recursivamente a árvore BSP e as salas
std::unique_ptr<BSPNode> splitSpace(Rect region, std::mt19937& rng, std::vector<Corridor>& corridors) {
    auto node = std::make_unique<BSPNode>();
    node->region = region;

    int w = region.right - region.left;
    int h = region.bottom - region.top;

    // Configuração dos tamanhos de partição
    constexpr int minSizeForSplit = 20; // Permite partições menores
    constexpr int maxLeafSize = 24;     // Impede que partições gigantes fiquem sem divisão

    // Se qualquer dimensão for maior que o máximo permitido, obriga o BSP a dividir
    bool forceSplit = (w > maxLeafSize || h > maxLeafSize);

    // OBRIGA A VIRAR SALA (FOLHA): Apenas se a área for pequena E não houver obrigação de divisão
    if (!forceSplit && (w < minSizeForSplit * 2 || h < minSizeForSplit * 2)) {
        int minPadding = 2; // Margem para paredes finas
        
        // Força o tamanho interno mínimo da sala para 10 tiles de altura e largura
        int roomW = std::max(20, w - (minPadding * 2));
        int roomH = std::max(20, h - (minPadding * 2));
        int roomL = region.left + minPadding;
        int roomT = region.top + minPadding;
        
        node->room = Rect{roomL, roomT, roomL + roomW, roomT + roomH};
        return node;
    }

    // Decide a direção da divisão priorizando o eixo mais longo
    bool splitH = (h > w); 
    if (w == h) {
        splitH = (rng() % 2 == 0);
    }

    // Garante limites seguros para o corte sem estourar o minSizeForSplit
    if (splitH) {
        int minBound = region.top + minSizeForSplit;
        int maxBound = region.bottom - minSizeForSplit;
        if (minBound > maxBound) minBound = maxBound;

        int splitCoord = minBound + (rng() % (maxBound - minBound + 1));

        Rect leftRegion{region.left, region.top, region.right, splitCoord};
        Rect rightRegion{region.left, splitCoord, region.right, region.bottom};

        node->left = splitSpace(leftRegion, rng, corridors);
        node->right = splitSpace(rightRegion, rng, corridors);
    } else {
        int minBound = region.left + minSizeForSplit;
        int maxBound = region.right - minSizeForSplit;
        if (minBound > maxBound) minBound = maxBound;

        int splitCoord = minBound + (rng() % (maxBound - minBound + 1));

        Rect leftRegion{region.left, region.top, splitCoord, region.bottom};
        Rect rightRegion{splitCoord, region.top, region.right, region.bottom};

        node->left = splitSpace(leftRegion, rng, corridors);
        node->right = splitSpace(rightRegion, rng, corridors);
    }

    // Nó interno: Conecta os filhos com um corredor
    auto getAnyRoom = [](const auto& self, const BSPNode* n) -> Rect {
        if (n->isLeaf()) return n->room;
        if (n->left) return self(self, n->left.get());
        return self(self, n->right.get());
    };
    
    Rect roomA = getAnyRoom(getAnyRoom, node->left.get());
    Rect roomB = getAnyRoom(getAnyRoom, node->right.get());

    int centerAX = (roomA.left + roomA.right) / 2;
    int centerAY = (roomA.top + roomA.bottom) / 2;
    int centerBX = (roomB.left + roomB.right) / 2;
    int centerBY = (roomB.top + roomB.bottom) / 2;

    // Cria corredor em L com 3 tiles de largura
    if (rng() % 2 == 0) {
        int minX = std::min(centerAX, centerBX);
        int maxX = std::max(centerAX, centerBX);
        corridors.push_back(Rect{minX, centerAY - 1, maxX + 1, centerAY + 2});

        int minY = std::min(centerAY, centerBY);
        int maxY = std::max(centerAY, centerBY);
        corridors.push_back(Rect{centerBX - 1, minY, centerBX + 2, maxY + 1});
    } else {
        int minY = std::min(centerAY, centerBY);
        int maxY = std::max(centerAY, centerBY);
        corridors.push_back(Rect{centerAX - 1, minY, centerAX + 2, maxY + 1});

        int minX = std::min(centerAX, centerBX);
        int maxX = std::max(centerAX, centerBX);
        corridors.push_back(Rect{minX, centerBY - 1, maxX + 1, centerBY + 2});
    }

    return node;
}

} // namespace

void Dungeon2D::generate(unsigned seed) {
    std::mt19937 rng(seed);
    corridors.clear();

    // Define a região inicial com margem de segurança
    Rect rootRegion{2, 2, width - 2, height - 2};
    root = splitSpace(rootRegion, rng, corridors);
}