#pragma once
#include "./Globals.hpp"
#include <raylib.h>
#include <unordered_set>
#include <vector>

class Player;
class SpatialGrid;

class Enemy {
  public:
    Vector2 pos;
    Vector2 dir;
    float radius;
    Circle hitCircle;
    Vector2 velocity;
    Vector2 knockbackVelocity;

    Texture2D texture;

    float damage;

    float speed;
    float knockbackResistance;

    float width;
    float height;

    std::unordered_set<int> gridCells;

    float health;

    float iframes;
    float iframeTimer;
    bool iframesReady;

    void Init();
    void Move(const Player &player);
    void Receive(Vector2 source, Circle damageSource, float force, float damage);
    void Die();
    void Update(const SpatialGrid &grid);
    void Avoid(const std::vector<Enemy *> &nearbyEnemies);
    void Draw();
};

extern std::vector<Enemy> enemies;
