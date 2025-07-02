#include "../include/Player.hpp"
#include <algorithm>
#include <raylib.h>
#include <raymath.h>

void Player::Init() {
    // player
    pos = {100, 100};
    dir = {0, 0};
    lastDir = {0, 0};

    colour = RAYWHITE;

    speed = 50.0;

    dashSpeed = 1500.0;
    dashDur = 0.2;
    dashCooldown = 1.0;
    dashTimer = 0.0;
    dashReady = true;

    friction = 4000.0;

    velocity = 200.0;
    currentVelocity = 0.0;
    maxVelocity = 600.0;
    moving = false;

    width = 15;
    height = 30;

    // camera
    camera = {0};
    camera.target = pos;
    camera.offset = {1920 / 2.0f, 1080 / 2.0f}; // TODO: make 1920/1080 variable names
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void Player::Move() {
    float delta = GetFrameTime();
    dashTimer -= delta;

    if (IsKeyPressed(KEY_LEFT_SHIFT) && dashReady) {
        dashTimer = dashDur;
        dashReady = false;
    }

    if (dashTimer > 0) {
        pos.x += lastDir.x * dashSpeed * delta;
        pos.y += lastDir.y * dashSpeed * delta;
    } else {
        if (dashTimer <= -dashCooldown) {
            dashReady = true;
        }

        dir = {0, 0};

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
            dir.y -= 1;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
            dir.y += 1;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            dir.x -= 1;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            dir.x += 1;

        if (dir.x != 0 || dir.y != 0) {
            dir = Vector2Normalize(dir);
            moving = true;
            lastDir = dir;
            currentVelocity += speed * velocity * delta;
            currentVelocity = std::min(currentVelocity, maxVelocity);
        } else {
            moving = false;
            currentVelocity -= friction * delta;
            currentVelocity = std::max(0.0f, currentVelocity);
        }

        pos.x += lastDir.x * currentVelocity * delta;
        pos.y += lastDir.y * currentVelocity * delta;

        currentVelocity = std::min(currentVelocity, maxVelocity);
    }

    camera.target = Vector2Lerp(camera.target, pos, 20.0f * delta);
}
