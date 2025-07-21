#include "../include/Player.hpp"
#include "../include/AnimationManager.hpp"
#include "../include/Enemy.hpp"
#include <algorithm>
#include <raylib.h>
#include <raymath.h>

void Player::Init() {
    animManager.AddAnimation(AnimState::IDLE, "../sprites/s_HuskOneIdle.png", 19, 0.1f, true);
    animManager.AddAnimation(AnimState::RUN, "../sprites/s_HuskOneRun.png", 6, 0.1f, true);
    currentAnimState = AnimState::IDLE;

    pos = {100.0f, 100.0f};
    dir = {0.0f, 0.0f};
    lastDir = {0.0f, 0.0f};
    cursorPos = {0.0f, 0.0f};

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

    damage = 5.0f;
    atkSpeed = 100.0f;
    atkCooldown = 0.5f;
    atkDuration = 0.5f;
    atkActiveTimer = 0.0f;
    atkCooldownTimer = 0.0f;
    atkReady = true;
    knockback = 500.0f;
    weaponRadius = 8.0f;
    weaponDistance = 15.0f;
    atkTexture = textures.huskOneWeaponTexture;

    health = 100.0f;
    iframes = 1.0f;
    iframeTimer = iframes;
    iframesReady = true;

    width = 10.0f;
    height = 22.0f;

    Vector2 center = {pos.x + width / 2.0f, pos.y + height / 2.0f};
    hitCircle = {center.x, center.y, 8.0f};
    atkCircle = {center.x, center.y, weaponRadius};

    camera = {0};
    camera.target = pos;
    camera.offset = {1920.0f / 2.0f, 1080.0f / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
}

void Player::Move() {
    float delta = GetFrameTime();
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

void Player::Attack(std::vector<Enemy> &enemies) {
    if (atkActiveTimer <= 0.0f) {
        return;
    }

    Vector2 playerCenter = {pos.x + width / 2.0f, pos.y + height / 2.0f};
    Vector2 direction = Vector2Subtract(cursorPos, playerCenter);
    Vector2 normalized = Vector2Normalize(direction);

    Vector2 weaponPosition = {playerCenter.x + normalized.x * weaponDistance,
                              playerCenter.y + normalized.y * weaponDistance};

    atkCircle.pos.x = weaponPosition.x;
    atkCircle.pos.y = weaponPosition.y;

    for (int i = 0; i < enemies.size();) {
        Enemy &enemy = enemies[i];
        if (CheckCollisionCircles(weaponPosition, atkCircle.radius, {enemy.hitCircle.pos.x, enemy.hitCircle.pos.y},
                                  enemy.hitCircle.radius)) {
            enemy.Receive(pos, atkCircle, knockback, damage);
            if (enemy.health <= 0.0f) {
                enemies.erase(enemies.begin() + i);
                continue;
            }
        }
        i++;
    }
}

void Player::Receive(std::vector<Enemy> &enemies) {
    float delta = GetFrameTime();

    if (!iframesReady) {
        iframeTimer -= delta;
        if (iframeTimer <= 0.0f) {
            iframesReady = true;
        }
    }

    if (iframesReady) {
        bool tookDamage = false;
        for (Enemy &enemy : enemies) {
            if (CheckCollisionCircles({hitCircle.pos.x, hitCircle.pos.y}, hitCircle.radius,
                                      {enemy.hitCircle.pos.x, enemy.hitCircle.pos.y}, enemy.hitCircle.radius)) {
                health -= enemy.damage;
                tookDamage = true;
                break;
            }
        }

        if (tookDamage) {
            iframesReady = false;
            iframeTimer = iframes;

            // knockback all enemies on hit
            for (Enemy &enemy : enemies) {
                Vector2 enemyCenter = {enemy.hitCircle.pos.x, enemy.hitCircle.pos.y};
                Vector2 playerCenter = {hitCircle.pos.x, hitCircle.pos.y};
                Vector2 direction = Vector2Subtract(enemyCenter, playerCenter);

                if (Vector2Length(direction) > 0.0f) {
                    direction = Vector2Normalize(direction);
                }

                enemy.knockbackVelocity = Vector2Scale(direction, knockback * 0.5f);
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
    Receive(enemies);
}

void Player::Draw() {
    DrawCircleV(cursorPos, 5.0f, RED);

    Vector2 direction = Vector2Subtract(cursorPos, pos);
    float rotation = atan2f(direction.y, direction.x) * RAD2DEG;

    if (atkActiveTimer > 0.0f) {
        DrawCircleLines(atkCircle.pos.x, atkCircle.pos.y, atkCircle.radius, RED);

        Vector2 txtOrigin = {(float)atkTexture.width / 2.0f, (float)atkTexture.height / 2.0f};
        Rectangle destRec = {atkCircle.pos.x, atkCircle.pos.y, (float)atkTexture.width, (float)atkTexture.height};
        DrawTexturePro(atkTexture, Rectangle{0.0f, 0.0f, (float)atkTexture.width, (float)atkTexture.height}, destRec,
                       txtOrigin, rotation, WHITE);
    }

    DrawCircleLines(hitCircle.pos.x, hitCircle.pos.y, hitCircle.radius, GREEN);

    bool flipSprite = (lastDir.x < 0.0f);
    animManager.Draw(pos, flipSprite);
}

void Player::ChangeAnimation(AnimState newState) {
    if (currentAnimState != newState) {
        currentAnimState = newState;
        animManager.Play(newState);
    }
}
