#pragma once
#include "./Weapon.hpp"

class Enemy;

class Player final : public Entity {
  public:
    Weapon weapon;

    void Init() override;
    void ChangeAnimation(AnimState newState) override;
    void Move();
    void Receive(std::vector<Enemy> &enemies) override;
    void Update() override;
    void Draw() override;
    void LevelUp();
};
