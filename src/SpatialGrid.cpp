#include "../include/SpatialGrid.hpp"
#include "../include/Globals.hpp"
#include <algorithm>
#include <cmath>
#include <unordered_set>

SpatialGrid grid(screenWidth, screenHeight, cellSize);

SpatialGrid::SpatialGrid(const float gameWidth, const float gameHeight, const float cellSize)
    : cellSize(cellSize), gridWidth(static_cast<int>(std::ceil(gameWidth / cellSize))),
      gridHeight(static_cast<int>(std::ceil(gameHeight / cellSize))) {
    cells.resize(gridWidth * gridHeight);
}

int SpatialGrid::Flatten(const int x, const int y) const { return y * gridWidth + x; }

void SpatialGrid::Clear() {
    for (auto &cell : cells) {
        cell.clear();
    }
}

void SpatialGrid::Insert(Enemy *enemy) {
    auto [minX, maxX, minY, maxY] = GetCellRange(enemy);

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
                cells[Flatten(x, y)].push_back(enemy);
                enemy->gridCells.insert(Flatten(x, y));
            }
        }
    }
}

void SpatialGrid::Update(Enemy *enemy) {
    for (const int cellIdx : enemy->gridCells) {
        auto &cell = cells[cellIdx];
        cell.erase(std::remove(cell.begin(), cell.end(), enemy), cell.end());
    }
    enemy->gridCells.clear();
    Insert(enemy);
}

void SpatialGrid::Remove(Enemy *enemy) {
    for (const int cellIdx : enemy->gridCells) {
        auto &cell = cells[cellIdx];
        cell.erase(std::remove(cell.begin(), cell.end(), enemy), cell.end());
    }
    enemy->gridCells.clear();
}

void SpatialGrid::Draw() const {
    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            DrawRectangleLines(cellSize * x, cellSize * y, cellSize, cellSize, ColorAlpha(GREEN, 0.3f));
            const int count = cells[Flatten(x, y)].size();
            DrawText(TextFormat("%d", count), x * cellSize + 5, y * cellSize + 5, 10, WHITE);
        }
    }
}

std::tuple<int, int, int, int> SpatialGrid::GetCellRange(const Enemy *enemy) const {
    const float left = enemy->GetPos().x - enemy->width / 2;
    const float right = enemy->GetPos().x + enemy->width / 2;
    const float top = enemy->GetPos().y - enemy->height  / 2;
    const float bottom = enemy->GetPos().y + enemy->height / 2;

    int minX = std::max(0, static_cast<int>(left / cellSize));
    int maxX = std::min(gridWidth - 1, static_cast<int>(right / cellSize));
    int minY = std::max(0, static_cast<int>(top / cellSize));
    int maxY = std::min(gridHeight - 1, static_cast<int>(bottom / cellSize));

    return std::make_tuple(minX, maxX, minY, maxY);
}

std::vector<Enemy *> SpatialGrid::GetPotentialCollisions(const Enemy *enemy) const {
    std::vector<Enemy *> potentials;
    std::unordered_set<Enemy *> uniquePotentials;

    auto [minX, maxX, minY, maxY] = GetCellRange(enemy);

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
                for (Enemy *other : cells[Flatten(x, y)]) {
                    if (other != enemy && uniquePotentials.insert(other).second) {
                        potentials.push_back(other);
                    }
                }
            }
        }
    }
    return potentials;
}

std::vector<Enemy *> SpatialGrid::GetNeighboursInRadius(const Enemy *enemy, const float radius) const {
    std::vector<Enemy *> neighbours;
    std::unordered_set<Enemy *> uniqueNeighbours;

    const float left = enemy->GetPos().x - radius;
    const float right = enemy->GetPos().x + radius;
    const float top = enemy->GetPos().y - radius;
    const float bottom = enemy->GetPos().y + radius;

    const int minX = std::max(0, static_cast<int>(left / cellSize));
    const int maxX = std::min(gridWidth - 1, static_cast<int>(right / cellSize));
    const int minY = std::max(0, static_cast<int>(top / cellSize));
    const int maxY = std::min(gridHeight - 1, static_cast<int>(bottom / cellSize));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            for (Enemy *other : cells[Flatten(x, y)]) {
                if (other != enemy && uniqueNeighbours.insert(other).second) {
                    neighbours.push_back(other);
                }
            }
        }
    }
    return neighbours;
}

std::vector<Enemy *> SpatialGrid::GetNeighbours(const Enemy *enemy) const {
    std::vector<Enemy *> nearby;
    std::unordered_set<Enemy *> uniqueEnemies;

    const int x = static_cast<int>(enemy->GetPos().x / cellSize);
    const int y = static_cast<int>(enemy->GetPos().y / cellSize);

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            const int nx = x + dx;
            const int ny = y + dy;
            if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
                for (Enemy *other : cells[Flatten(nx, ny)]) {
                    if (other != enemy && uniqueEnemies.insert(other).second) {
                        nearby.push_back(other);
                    }
                }
            }
        }
    }
    return nearby;
}
