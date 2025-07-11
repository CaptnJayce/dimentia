#include "../include/Enemy.hpp"
#include "../include/Player.hpp"
#include <raylib.h>
#include <raymath.h>

std::vector<Enemy> enemies;

void Enemy::Init() {
    // TODO: hitbox
    pos = {200, 200};
    dir = {0, 0};

    texture = LoadTexture("../sprites/abominable_mass.png");

    speed = 80.0;

    width = 28.0;
    height = 13.0;

    hitbox = {0, 0, width, height};

    damage = 10.0;
    health = 10.0;

    iframes = 1.0; // invuln for 1 second
    iframeTimer = iframes;
    iframesReady = true;

    velocity = {0, 0};
    knockbackVelocity = {0, 0};
    knockbackResistance = 0.0;
}

void Enemy::Move(const Player &player) {
    float delta = GetFrameTime();

    // good nuff
    dir = Vector2Normalize(player.pos - pos);
    pos.x += dir.x * speed * delta;
    pos.y += dir.y * speed * delta;

    hitbox.x = pos.x;
    hitbox.y = pos.y;
}

void Enemy::Receive(Vector2 source, Rectangle damageSource, float knockback, float damage) {
    Vector2 enemyCenter = {hitbox.x + hitbox.width / 2, hitbox.y + hitbox.height / 2};

    Vector2 direction = Vector2Subtract(enemyCenter, source);
    if (Vector2Length(direction) > 0) {
        direction = Vector2Normalize(direction);
    }

    if (iframesReady) {
        knockbackVelocity = Vector2Scale(direction, knockback);
        health -= damage;
        iframesReady = false;
        iframeTimer = iframes;
    }
}

void Enemy::Die() {
    for (auto it = enemies.begin(); it != enemies.end();) {
        if ((*it).health <= 0) {
            UnloadTexture((*it).texture);
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void Enemy::Update() {
    float delta = GetFrameTime();

    if (!iframesReady) {
        iframeTimer -= delta;
        if (iframeTimer <= 0.0f) {
            iframesReady = true;
        }
    }

    if (Vector2Length(knockbackVelocity) > 0.1f) {
        pos = Vector2Add(pos, Vector2Scale(knockbackVelocity, delta));
        knockbackVelocity = Vector2Lerp(knockbackVelocity, Vector2Zero(), 5.0f * delta);
    } else {
        knockbackVelocity = Vector2Zero();
    }

    hitbox.x = pos.x;
    hitbox.y = pos.y;
}

void Enemy::Draw() {
    DrawRectangleLinesEx(hitbox, 1.0f, RED);

    Rectangle rect = {0, 0, (float)texture.width, (float)texture.height};
    DrawTextureRec(texture, rect, pos, RAYWHITE);
}
