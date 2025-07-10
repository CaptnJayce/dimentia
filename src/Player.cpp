#include "../include/Player.hpp"
#include "../include/AnimationManager.hpp"
#include "../include/Enemy.hpp"
#include "../include/Utils.hpp"
#include <algorithm>
#include <raylib.h>
#include <raymath.h>

void Player::Init() {
    // TODO: move anims into own Init function
    animManager.AddAnimation(AnimState::IDLE, "../sprites/s_HuskOneIdle.png", 19, 0.1, true);
    animManager.AddAnimation(AnimState::RUN, "../sprites/s_HuskOneRun.png", 6, 0.1, true);
    currentAnimState = AnimState::IDLE;

    // player
    pos = {100, 100};
    dir = {0, 0};
    lastDir = {0, 0};
    cursorPos = {0, 0};

    speed = 10.0;

    damage = 5.0;
    atkSpeed = 100.0;
    atkCooldown = 0.5;
    atkDuration = 0.5;
    atkActiveTimer = 0.0;
    atkCooldownTimer = 0.0;
    atkReady = true;
    atkTexture = LoadTexture("../sprites/crescent_slash.png");
    atkHitbox = {0, 0, 4, 16};
    knockback = 500.0;

    dashSpeed = 600.0;
    dashDur = 0.2;
    dashCooldown = 1.0;
    dashTimer = 0.0;
    dashReady = true;
    friction = 4000.0;
    velocity = 200.0;
    currentVelocity = 0.0;
    maxVelocity = 200.0;
    moving = false;

    width = frameWidth;
    height = frameHeight;
    health = 50.0;

    hitbox = {pos.x, pos.y, width, height};

    camera = {0};
    camera.target = pos;
    camera.offset = {1920 / 2.0, 1080 / 2.0};
    camera.rotation = 0.0;
    camera.zoom = 4.0f;
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
            ChangeAnimation(AnimState::RUN);
        } else {
            moving = false;
            currentVelocity -= friction * delta;
            currentVelocity = std::max(0.0f, currentVelocity);
            ChangeAnimation(AnimState::IDLE);
        }

        pos.x += lastDir.x * currentVelocity * delta;
        pos.y += lastDir.y * currentVelocity * delta;

        hitbox.x = pos.x;
        hitbox.y = pos.y;

        currentVelocity = std::min(currentVelocity, maxVelocity);
    }

    camera.target = Vector2Lerp(camera.target, pos, 20.0f * delta);
}

void Player::Attack(std::vector<Enemy> &enemies) {
    if (atkActiveTimer > 0.0f) {
        for (auto &enemy : enemies) {
            if (CheckCollisionRecs(atkHitbox, enemy.hitbox)) {
                enemy.Knockback(pos, knockback);
            }
        }
    }
}

void Player::Update(std::vector<Enemy> &enemies) {
    float delta = GetFrameTime();
    animManager.Update();

    cursorPos = GetScreenToWorld2D(GetMousePosition(), camera);

    atkActiveTimer -= delta;
    atkCooldownTimer -= delta;

    if (atkCooldownTimer <= 0.0f) {
        atkReady = true;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && atkReady) {
        atkActiveTimer = atkDuration;
        atkCooldownTimer = atkDuration + atkCooldown;
        atkReady = false;
    }

    Attack(enemies);

    for (const auto &enemy : enemies) {
        bool isColliding = Utils::CheckRectangles(hitbox, enemy.hitbox);
        if (isColliding) {
            // TODO: remove player health & add iframes
            break;
        }
    }
}

void Player::Draw() {
    // cursor
    DrawCircleV(cursorPos, 5.0f, RED);

    Vector2 direction = Vector2Subtract(cursorPos, pos);
    float rotation = atan2f(direction.y, direction.x) * RAD2DEG;

    Vector2 playerCenter = {pos.x + frameWidth / 2.0f, pos.y + frameHeight / 2.0f};

    // atk
    const float maxRadius = 30.0f;

    Vector2 atkPosition;
    Vector2 normalized = Vector2Normalize(direction);
    atkPosition = Vector2Add(playerCenter, Vector2Scale(normalized, maxRadius));

    if (atkActiveTimer > 0.0f) {
        atkHitbox.width = (float)atkTexture.width;
        atkHitbox.height = (float)atkTexture.height;

        Vector2 txtOrigin = {atkHitbox.width / 2, atkHitbox.height / 2};

        Rectangle rotHitbox = {atkPosition.x, atkPosition.y, atkHitbox.width, atkHitbox.height};
        DrawRectanglePro(rotHitbox, txtOrigin, rotation, RED);

        Rectangle destRec = {atkPosition.x, atkPosition.y, (float)atkTexture.width, (float)atkTexture.height};
        DrawTexturePro(atkTexture, Rectangle{0, 0, (float)atkTexture.width, (float)atkTexture.height}, destRec,
                       txtOrigin, rotation, WHITE);

        atkHitbox = rotHitbox;
    }

    bool flipSprite = (lastDir.x < 0);
    animManager.Draw(pos, flipSprite);
}

void Player::ChangeAnimation(AnimState newState) {
    if (currentAnimState != newState) {
        currentAnimState = newState;
        animManager.Play(newState);
    }
}
