#pragma once
#include "./Globals.hpp"
#include <raylib.h>
#include <vector>

class Player;

// TODO: make it so enemies dont overlap with each other
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

    // TODO: give enemies slightly random speed
    float speed;
    float knockbackResistance;

    float width;
    float height;

    float health;

    float iframes;
    float iframeTimer;
    bool iframesReady;

    void Init();
    void Move(const Player &player);
    void Update();
    void Receive(Vector2 source, Circle damageSource, float force, float damage);
    void Die();
    void Draw();
};

extern std::vector<Enemy> enemies;
