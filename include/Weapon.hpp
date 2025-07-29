#pragma once
#include "./Entity.hpp"

class Player;

class Weapon final : public Entity {
  public:
    void Attack(std::vector<Enemy> &enemies) override;
    void Init() override;
    void Update(const Player& player);
    void Draw() override;
};