#pragma once
#include <raylib.h>

class Enemy {
  public:
    Vector2 pos;

    Color colour; // TODO: replace with texture
    // Texture2D texture;

    float speed;

    int width;
    int height;

    void Init();
    void Move();
    void Draw();
};
