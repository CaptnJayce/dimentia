#pragma once
#include "./Player.hpp"
#include <raylib.h>

class Enemy {
  public:
    Vector2 pos;
    Vector2 dir;

    Texture2D texture;

    float speed;

    int width;
    int height;

    void Init();
    void Move(const Player &player);
    void Draw();
};
