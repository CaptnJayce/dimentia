#pragma once
#include <raylib.h>

class Player {
  public:
    Vector2 pos;
    Vector2 dir;
    Vector2 lastDir;

    Texture2D texture;

    float speed;

    float friction;

    float dashSpeed;
    float dashDur;
    float dashCooldown;
    float dashTimer;
    bool dashReady;

    float velocity;
    float currentVelocity;
    float maxVelocity;

    bool moving;

    int width;
    int height;

    // camera : may put in own header later
    Camera2D camera;
    float zoom;
    float offset;
    Vector2 target;

    void Init();
    void Move();
    void Draw();
};
