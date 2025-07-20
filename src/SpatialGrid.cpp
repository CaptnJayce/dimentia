#include "../include/SpatialGrid.hpp"
#include <algorithm>
#include <cmath>
#include <unordered_set>

SpatialGrid::SpatialGrid(float gameWidth, float gameHeight, float cellSize)
    : cellSize(cellSize), gridWidth(static_cast<int>(std::ceil(gameWidth / cellSize))),
      gridHeight(static_cast<int>(std::ceil(gameHeight / cellSize))) {
    cells.resize(gridWidth * gridHeight);
}

int SpatialGrid::Flatten(int x, int y) const { return y * gridWidth + x; }

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
    for (int cellIdx : enemy->gridCells) {
        auto &cell = cells[cellIdx];
        cell.erase(std::remove(cell.begin(), cell.end(), enemy), cell.end());
    }
    enemy->gridCells.clear();
    Insert(enemy);
}

void SpatialGrid::Remove(Enemy *enemy) {
    for (int cellIdx : enemy->gridCells) {
        auto &cell = cells[cellIdx];
        cell.erase(std::remove(cell.begin(), cell.end(), enemy), cell.end());
    }
    enemy->gridCells.clear();
}

void SpatialGrid::Draw() {
    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            DrawRectangleLines(x * cellSize, y * cellSize, cellSize, cellSize, ColorAlpha(GREEN, 0.3f));
            int count = cells[Flatten(x, y)].size();
            DrawText(TextFormat("%d", count), x * cellSize + 5, y * cellSize + 5, 10, WHITE);
        }
    }
}

std::tuple<int, int, int, int> SpatialGrid::GetCellRange(Enemy *enemy) const {
    float left = enemy->pos.x - enemy->width / 2;
    float right = enemy->pos.x + enemy->width / 2;
    float top = enemy->pos.y - enemy->height / 2;
    float bottom = enemy->pos.y + enemy->height / 2;

    int minX = std::max(0, static_cast<int>(left / cellSize));
    int maxX = std::min(gridWidth - 1, static_cast<int>(right / cellSize));
    int minY = std::max(0, static_cast<int>(top / cellSize));
    int maxY = std::min(gridHeight - 1, static_cast<int>(bottom / cellSize));

    return std::make_tuple(minX, maxX, minY, maxY);
}

std::vector<Enemy *> SpatialGrid::GetPotentialCollisions(Enemy *enemy) {
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

std::vector<Enemy *> SpatialGrid::GetNeighboursInRadius(Enemy *enemy, float radius) const {
    std::vector<Enemy *> neighbours;
    std::unordered_set<Enemy *> uniqueNeighbours;

    float left = enemy->pos.x - radius;
    float right = enemy->pos.x + radius;
    float top = enemy->pos.y - radius;
    float bottom = enemy->pos.y + radius;

    int minX = std::max(0, static_cast<int>(left / cellSize));
    int maxX = std::min(gridWidth - 1, static_cast<int>(right / cellSize));
    int minY = std::max(0, static_cast<int>(top / cellSize));
    int maxY = std::min(gridHeight - 1, static_cast<int>(bottom / cellSize));

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

std::vector<Enemy *> SpatialGrid::GetNeighbours(Enemy *enemy) {
    std::vector<Enemy *> nearby;
    std::unordered_set<Enemy *> uniqueEnemies;

    int x = static_cast<int>(enemy->pos.x / cellSize);
    int y = static_cast<int>(enemy->pos.y / cellSize);

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = x + dx;
            int ny = y + dy;
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
