#include "../include/Weapon.hpp"
#include "../include/Enemy.hpp"
#include "../include/Globals.hpp"
#include "../include/Player.hpp"
#include "raymath.h"
#include <raylib.h>

void Weapon::Init() {
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
}

void Weapon::Attack(std::vector<Enemy> &enemies) {
    if (atkActiveTimer <= 0.0f) {
        return;
    }

    const Vector2 direction = Vector2Subtract(cursorPos, pos);
    const Vector2 normalized = Vector2Normalize(direction);

    const Vector2 weaponPosition = {
        pos.x + normalized.x * weaponDistance,
        pos.y + normalized.y * weaponDistance
    };

    atkCircle.pos = weaponPosition;

    for (int i = 0; i < enemies.size();) {
        if (Enemy &enemy = enemies[i]; CheckCollisionCircles(
            weaponPosition, atkCircle.radius,
            {enemy.GetHitCircle().pos.x, enemy.GetHitCircle().pos.y},
            enemy.GetHitCircle().radius
        )) {
            enemy.Receive(pos, atkCircle, knockback, damage);
            if (enemy.GetHealth() <= 0.0f) {
                enemies.erase(enemies.begin() + i);
                continue;
            }
        }
        i++;
    }
}

void Weapon::Draw() {
    const Vector2 direction = Vector2Subtract(cursorPos, pos);
    const float rotation = atan2f(direction.y, direction.x) * RAD2DEG;

    if (atkActiveTimer > 0.0f) {
        DrawCircleLines(atkCircle.pos.x, atkCircle.pos.y, atkCircle.radius, RED);

        // good lord
        const Vector2 txtOrigin = {static_cast<float>(atkTexture.width) / 2.0f, static_cast<float>(atkTexture.height) / 2.0f};
        const Rectangle destRec = {atkCircle.pos.x, atkCircle.pos.y, static_cast<float>(atkTexture.width), static_cast<float>(atkTexture.height)};
        DrawTexturePro(atkTexture, Rectangle{0.0f, 0.0f, static_cast<float>(atkTexture.width), static_cast<float>(atkTexture.height)}, destRec,
                       txtOrigin, rotation, WHITE);
    }

    const bool flipSprite = (lastDir.x < 0.0f);
    animManager.Draw(pos, flipSprite);
}

void Weapon::Update(const Player& player) {
    const float delta = GetFrameTime();
    cursorPos = GetScreenToWorld2D(GetMousePosition(), player.camera);

    const Vector2 playerCenter = {
        player.GetPos().x + player.width / 2.0f,
        player.GetPos().y + player.height / 2.0f
    };
    pos = playerCenter;

    // needed for debug
    atkCircle = {pos.x, pos.y, weaponRadius};

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
}