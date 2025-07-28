#pragma once
#include "./Entity.hpp"

#include <raylib.h>
#include <unordered_set>
#include <vector>

class Player;

class Enemy final : public Entity {
  public:
    // TODO: move this out of enemy
    std::unordered_set<int> gridCells;

    void Init() override;
    void Move(const Player & player);
    void Receive(Vector2 source, Circle damageSource, float knockback, float damage);
    void Die() override;
    void Update() override;
    void Avoid(const std::vector<Enemy *> &nearbyEnemies);
    void Draw() const;
};

extern std::vector<Enemy> enemies;
