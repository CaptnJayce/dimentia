#include "../include/SpatialGrid.hpp"

SpatialGrid::SpatialGrid(float gameWidth, float gameHeight, float cellSize) : cellSize(cellSize) {
    gridWidth = gameWidth / cellSize;
    gridHeight = gameHeight / cellSize;
    cells.resize(gridWidth * gridHeight);
}

int SpatialGrid::Flatten(int x, int y) const { return y * gridWidth + x; }

void SpatialGrid::Clear() {
    for (std::vector<Enemy *> &cell : cells) {
        cell.clear();
    }
}

void SpatialGrid::Insert(Enemy *enemy) {
    int x = (enemy->pos.x / cellSize);
    int y = (enemy->pos.y / cellSize);

    if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight) {
        cells[Flatten(x, y)].push_back(enemy);
    }
}

std::vector<Enemy *> SpatialGrid::GetNeighbours(Enemy *enemy) {
    std::vector<Enemy *> nearby;
    int x = enemy->pos.x / cellSize;
    int y = enemy->pos.y / cellSize;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
                std::vector<Enemy *> &cell = cells[Flatten(nx, ny)];
                nearby.insert(nearby.end(), cell.begin(), cell.end());
            }
        }
    }
    return nearby;
}
