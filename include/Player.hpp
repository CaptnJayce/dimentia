#pragma once
#include "./AnimationManager.hpp"
#include "./Entity.hpp"

#include <vector>

class Enemy;

class Player final : public Entity {
  public:
    void Init() override;
    void ChangeAnimation(AnimState newState) override;
    void Move();
    void Attack(std::vector<Enemy> &enemies);
    void Receive(std::vector<Enemy> &enemies);
    void Update();
    void Draw() override;

    Camera2D camera{};
    float zoom{};
    float offset{};
    Vector2 target{};
};
