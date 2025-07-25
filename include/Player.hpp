#pragma once
#include "./AnimationManager.hpp"
#include "./Globals.hpp"

#include <raylib.h>
#include <vector>

class Enemy;

class Player {
  public:
    void Init();
    void ChangeAnimation(AnimState newState);
    void Move();
    void Attack(std::vector<Enemy> &enemies);
    void Receive(std::vector<Enemy> &enemies);
    void Update(std::vector<Enemy> &enemies);
    void Draw();

    // TODO should probably make these private at some point
    Vector2 pos;
    Vector2 dir;
    Vector2 lastDir;
    Circle hitCircle;

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
    float radius;
    float weaponRadius;
    float weaponDistance;
    Circle atkCircle;

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
    float iframes;
    float iframeTimer;
    bool iframesReady;

    Camera2D camera;
    float zoom;
    float offset;
    Vector2 target;

    AnimationManager animManager;
    AnimState currentAnimState;
};
