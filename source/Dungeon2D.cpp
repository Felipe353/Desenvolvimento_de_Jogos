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

    // Tamanho mínimo para tentar uma divisão (precisa caber pelo menos duas salas de 5x5 + margens)
    constexpr int minSizeForSplit = 20;

    // Decide a direção da divisão com base nas proporções ou aleatoriamente se forem parecidas
    bool splitH = (h > w); // se a altura for maior que a largura, prefere dividir horizontalmente
    if (w == h) {
        splitH = (rng() % 2 == 0);
    }

    // Verifica se a região é muito pequena para continuar dividindo
    if (splitH && h < minSizeForSplit * 2) {
        splitH = false;
    }
    if (!splitH && w < minSizeForSplit * 2) {
        // Se ambos forem pequenos demais para dividir, vira nó folha
        if (h < minSizeForSplit * 2) {
            // Para a recursão aqui
            // Cria a sala dentro da região folha
            int roomW = std::max(5, w - 4);
            int roomH = std::max(5, h - 4);
            int roomL = region.left + 1 + (w - roomW > 2 ? (rng() % (w - roomW - 1)) : 0);
            int roomT = region.top + 1 + (h - roomH > 2 ? (rng() % (h - roomH - 1)) : 0);
            node->room = Rect{roomL, roomT, roomL + roomW, roomT + roomH};
            return node;
        }
    }

    if (splitH) {
        // Divisão horizontal (corte na altura)
        int splitCoord = region.top + minSizeForSplit + (rng() % (h - minSizeForSplit * 2));
        Rect leftRegion{region.left, region.top, region.right, splitCoord};
        Rect rightRegion{region.left, splitCoord, region.right, region.bottom};

        node->left = splitSpace(leftRegion, rng, corridors);
        node->right = splitSpace(rightRegion, rng, corridors);
    } else {
        // Divisão vertical (corte na largura)
        int splitCoord = region.left + minSizeForSplit + (rng() % (w - minSizeForSplit * 2));
        Rect leftRegion{region.left, region.top, splitCoord, region.bottom};
        Rect rightRegion{splitCoord, region.top, region.right, region.bottom};

        node->left = splitSpace(leftRegion, rng, corridors);
        node->right = splitSpace(rightRegion, rng, corridors);
    }

    // Nó interno: conecta os filhos com um corredor de 3 tiles de largura
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