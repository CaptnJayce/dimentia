#pragma once
#include <raylib.h>
#include <vector>

class Enemy;

class Player {
  public:
    Vector2 pos;
    Vector2 dir;
    Vector2 lastDir;
    Rectangle hitbox;

    Texture2D idle;
    Texture2D run;
    Texture2D currentAnim;

    int frameDelay;
    int frameCounter;
    int animFrames;
    int currentAnimFrame;
    int frameWidth;
    int frameHeight;

    float speed;

    float friction;

    Vector2 cursorPos;

    float damage;
    float atkSpeed;
    float atkCooldown;
    float atkDuration;
    float atkActiveTimer;
    float atkCooldownTimer;
    float knockback;
    bool atkReady;
    Texture2D atkTexture;
    Rectangle atkHitbox;

    float dashSpeed;
    float dashDur;
    float dashCooldown;
    float dashTimer;
    bool dashReady;

    float velocity;
    float currentVelocity;
    float maxVelocity;

    bool moving;

    float width;
    float height;

    float health;

    // camera : may put in own header later
    Camera2D camera;
    float zoom;
    float offset;
    Vector2 target;

    void Init();

    // actions
    void Move();
    void Attack(std::vector<Enemy> &enemies);

    // rendering stuff
    void Update(std::vector<Enemy> &enemies);
    void Draw();
};
