#pragma once
#include "./Enemy.hpp"
#include <vector>

class SpatialGrid {
  private:
    float cellSize;
    float gridWidth;
    float gridHeight;
    std::vector<std::vector<Enemy *>> cells;

    // convert 2D grid into 1D
    int Flatten(int x, int y) const;

  public:
    // init grid based on world size
    SpatialGrid(float gameWidth, float gameHeight, float cellSize);

    void Clear();
    void Insert(Enemy *enemy);

    // get enemies cell & adjacent cells (3x3 area for now)
    std::vector<Enemy *> GetNeighbours(Enemy *enemy);
};
