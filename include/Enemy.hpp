#pragma once
#include <raylib.h>

class Player;

// TODO: make it so enemies dont overlap with each other
class Enemy {
  public:
    Vector2 pos;
    Vector2 dir;
    Rectangle hitbox;
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

    void Init();
    void Move(const Player &player);
    void Update();
    void Draw();
    void Knockback(Vector2 source, float force);
};
