#pragma once
#include "./Entity.hpp"

class Player;

class Weapon final : public Entity {
  public:
    void Attack(std::vector<Enemy> &enemies, Player &player);
    void Init() override;
    void Update(Player &player);
    void Draw() override;
};