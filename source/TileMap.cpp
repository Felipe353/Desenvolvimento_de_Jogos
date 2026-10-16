#include "TileMap.hpp"
#include "Dungeon2D.hpp"
#include "Global.hpp"

#include <fstream>
#include <sstream>
#include <random>
#include <cmath>

void TileMap::loadFromDungeon(const Dungeon2D& dungeon) {
    m_width = dungeon.width;
    m_height = dungeon.height;
    m_cells.assign(m_width * m_height, WALL_TILE);

    // Função recursiva para percorrer a árvore BSP e rasterizar as salas
    auto carveRooms = [this](auto& self, const BSPNode* node) -> void {
        if (!node) return;
        if (node->isLeaf()) {
            const Rect& r = node->room;
            for (int row = r.top; row < r.bottom; ++row) {
                for (int col = r.left; col < r.right; ++col) {
                    if (col >= 0 && col < m_width && row >= 0 && row < m_height) {
                        m_cells[row * m_width + col] = EMPTY_TILE;
                    }
                }
            }
        } else {
            self(self, node->left.get());
            self(self, node->right.get());
        }
    };

    if (dungeon.root) {
        carveRooms(carveRooms, dungeon.root.get());
    }

    // Rasteriza todos os corredores
    for (const auto& corridor : dungeon.corridors) {
        for (int row = corridor.top; row < corridor.bottom; ++row) {
            for (int col = corridor.left; col < corridor.right; ++col) {
                if (col >= 0 && col < m_width && row >= 0 && row < m_height) {
                    m_cells[row * m_width + col] = EMPTY_TILE;
                }
            }
        }
    }
}

bool TileMap::saveCSV(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }

    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            file << m_cells[row * m_width + col];
            if (col < m_width - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    return true;
}

bool TileMap::loadCSV(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    std::vector<int> tempCells;
    std::string line;
    int rows = 0;
    int cols = -1;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string cellValStr;
        int currentCols = 0;
        std::vector<int> rowCells;

        while (std::getline(ss, cellValStr, ',')) {
            try {
                rowCells.push_back(std::stoi(cellValStr));
                currentCols++;
            } catch (...) {
                return false; // Erro de conversão
            }
        }

        if (cols == -1) {
            cols = currentCols;
        } else if (currentCols != cols) {
            return false; // Linhas com larguras inconsistentes
        }

        tempCells.insert(tempCells.end(), rowCells.begin(), rowCells.end());
        rows++;
    }

    if (rows == 0 || cols <= 0) {
        return false;
    }

    m_width = cols;
    m_height = rows;
    m_cells = std::move(tempCells);

    return true;
}

int TileMap::width() const noexcept {
    return m_width;
}

int TileMap::height() const noexcept {
    return m_height;
}

int TileMap::at(int col, int row) const {
    return m_cells[row * m_width + col];
}

bool TileMap::isWall(int col, int row) const {
    return at(col, row) == WALL_TILE;
}

const int* TileMap::data() const noexcept {
    return m_cells.data();
}

void TileMap::toIndices(const Vector2D& world, int& col, int& row) const noexcept {
    col = static_cast<int>(std::floor(world.x / TILE_SIZE));
    row = static_cast<int>(std::floor(world.y / TILE_SIZE));
}

Vector2D TileMap::toWorld(int col, int row) const noexcept {
    float x = (static_cast<float>(col) + 0.5f) * TILE_SIZE;
    float y = (static_cast<float>(row) + 0.5f) * TILE_SIZE;
    return Vector2D{x, y};
}

void TileMap::randomEmptyCell(int& col, int& row) const {
    // Coleta todas as posições vazias para selecionar uma de forma aleatória uniforme
    std::vector<std::pair<int, int>> emptyCells;
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            if (m_cells[r * m_width + c] == EMPTY_TILE) {
                emptyCells.emplace_back(c, r);
            }
        }
    }

    if (emptyCells.empty()) {
        col = 0;
        row = 0;
        return;
    }

    // Usa gerador de números aleatórios padrão
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, emptyCells.size() - 1);

    auto chosen = emptyCells[dis(gen)];
    col = chosen.first;
    row = chosen.second;
}