#include "../include/Player.hpp"
#include "../include/AnimationManager.hpp"
#include "../include/Enemy.hpp"
#include "../include/Weapon.hpp"
#include "../include/Globals.hpp"
#include <algorithm>
#include <raylib.h>
#include <raymath.h>

void Player::Init() {
    weapon = Weapon();

    animManager.AddAnimation(AnimState::IDLE, textures.crescentWitchIdleTexture, 19, 0.1f, true);
    animManager.AddAnimation(AnimState::RUN, textures.crescentWitchRunTexture, 6, 0.1f, true);
    currentAnimState = AnimState::IDLE;

    pos = {0.0f, 0.0f};
    dir = {0.0f, 0.0f};
    lastDir = {0.0f, 0.0f};
    cursorPos = {0.0f, 0.0f};

    level = 0;
    expTotal = 0.0f;
    // TODO: implement formula for handling level up threshold
    expThreshold = 100.0f;

    speed = 10.0f;
    dashSpeed = 300.0f;
    dashDur = 0.2f;
    dashCooldown = 3.0f;
    dashTimer = 0.0f;
    dashReady = true;
    friction = 4000.0f;
    velocity = 200.0f;
    currentVelocity = 0.0f;
    maxVelocity = 100.0f;
    moving = false;

    knockback = 500.0f;

    health = 5.0f;
    iframes = 1.0f;
    iframeTimer = iframes;
    iframesReady = true;

    width = 10.0f;
    height = 22.0f;

    center = {pos.x + width / 2.0f, pos.y + height / 2.0f};
    hitCircle = {center.x, center.y, 8.0f};

    camera = {0};
    camera.target = pos;
    camera.offset = {1920.0f / 2.0f, 1080.0f / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
}

void Player::Move() {
    const float delta = GetFrameTime();
    dashTimer -= delta;

    if (IsKeyPressed(KEY_LEFT_SHIFT) && dashReady) {
        dashTimer = dashDur;
        dashReady = false;
    }

    if (dashTimer > 0.0f) {
        pos.x += lastDir.x * dashSpeed * delta;
        pos.y += lastDir.y * dashSpeed * delta;
    } else {
        if (dashTimer <= -dashCooldown) {
            dashReady = true;
        }

        dir = {0.0f, 0.0f};
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
            dir.y -= 1.0f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
            dir.y += 1.0f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            dir.x -= 1.0f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            dir.x += 1.0f;

        if (dir.x != 0.0f || dir.y != 0.0f) {
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

        currentVelocity = std::min(currentVelocity, maxVelocity);
    }

    hitCircle.pos.x = pos.x + width / 2.0f;
    hitCircle.pos.y = pos.y + height / 2.0f;

    camera.target = Vector2Lerp(camera.target, pos, 20.0f * delta);
}

void Player::Receive(std::vector<Enemy> &enemies) {
    const float delta = GetFrameTime();

    if (!iframesReady) {
        iframeTimer -= delta;
        if (iframeTimer <= 0.0f) {
            iframesReady = true;
        }
    }

    if (iframesReady) {
        bool tookDamage = false;
        for (const Enemy &enemy : enemies) {
            if (CheckCollisionCircles(
                {hitCircle.pos.x, hitCircle.pos.y}, hitCircle.radius,
                {enemy.GetHitCircle().pos.x, enemy.GetHitCircle().pos.y},
                enemy.GetHitCircle().radius
            )) {
                health -= enemy.GetDamage();
                tookDamage = true;
                break;
            }
        }


        if (tookDamage) {
            iframesReady = false;
            iframeTimer = iframes;

            for (Enemy &enemy : enemies) {
                const Vector2 enemyCenter = {
                    enemy.GetHitCircle().pos.x,
                    enemy.GetHitCircle().pos.y
                };
                const Vector2 playerCenter = {hitCircle.pos.x, hitCircle.pos.y};
                Vector2 direction = Vector2Subtract(enemyCenter, playerCenter);

                if (Vector2Length(direction) > 0.0f) {
                    direction = Vector2Normalize(direction);
                }

                enemy.SetKnockbackVelocity(Vector2Scale(direction, knockback * 0.5f));
            }
        }

    }
}

void Player::Update() {
    animManager.Update();

    Receive(enemies);
}

void Player::Draw() {
    const bool flipSprite = (lastDir.x < 0.0f);
    animManager.Draw(pos, flipSprite);
}

void Player::ChangeAnimation(const AnimState newState) {
    if (currentAnimState != newState) {
        currentAnimState = newState;
        animManager.Play(newState);
    }
}
