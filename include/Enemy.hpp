#pragma once
#include "./Entity.hpp"

#include <raylib.h>
#include <vector>

class Player;

class Enemy final : public Entity {
public:
  void Init() override;
  void Move(const Player &player);
  void Receive(Vector2 source, float knockback, float damage, Player &player);
  void Die(Player &player) override;
  void Update() override;
  void Avoid(const std::vector<Enemy *> &nearbyEnemies);
  void Draw() const;

  float expDrop{};
};

extern std::vector<Enemy> enemies;
