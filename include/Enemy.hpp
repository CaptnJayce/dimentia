#pragma once
#include <raylib.h>

class Player;

class Enemy {
  public:
    Vector2 pos;
    Vector2 dir;
    Rectangle hitbox;

    Texture2D texture;

    float speed;

    float width;
    float height;

    float health;

    void Init();
    void Move(const Player &player);
    void Update();
    void Draw();
};
