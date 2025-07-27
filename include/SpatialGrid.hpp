#pragma once
#include "./Enemy.hpp"
#include <vector>

class Enemy;

class SpatialGrid {
  private:
    float cellSize;
    int gridWidth;
    int gridHeight;
    std::vector<std::vector<Enemy *>> cells;

    int Flatten(int x, int y) const;

    std::tuple<int, int, int, int> GetCellRange(const Enemy *enemy) const;

  public:
    SpatialGrid(float gameWidth, float gameHeight, float cellSize);

    void Clear();
    void Insert(Enemy *enemy);
    void Update(Enemy *enemy);
    void Draw() const;
    void Remove(Enemy *enemy);

    std::vector<Enemy *> GetNeighbours(const Enemy *enemy) const;
    std::vector<Enemy *> GetPotentialCollisions(const Enemy *enemy) const;
    std::vector<Enemy *> GetNeighboursInRadius(const Enemy *enemy, float radius) const;
};
