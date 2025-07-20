#pragma once
#include "./Enemy.hpp"
#include <vector>

class SpatialGrid {
  private:
    float cellSize;
    int gridWidth;
    int gridHeight;
    std::vector<std::vector<Enemy *>> cells;

    int Flatten(int x, int y) const;

    std::tuple<int, int, int, int> GetCellRange(Enemy *enemy) const;

  public:
    SpatialGrid(float gameWidth, float gameHeight, float cellSize);

    void Clear();
    void Insert(Enemy *enemy);
    void Update(Enemy *enemy);
    void Draw();
    void Remove(Enemy *enemy);

    std::vector<Enemy *> GetNeighbours(Enemy *enemy);
    std::vector<Enemy *> GetPotentialCollisions(Enemy *enemy);
    std::vector<Enemy *> GetNeighboursInRadius(Enemy *enemy, float radius) const;
};
