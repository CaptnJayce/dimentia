#include "../include/Enemy.hpp"
#include "../include/Player.hpp"
#include <raylib.h>
#include <raymath.h>

std::vector<Enemy> enemies;

void Enemy::Init() {
    pos = {200.0f, 200.0f};
    dir = {0.0f, 0.0f};
    texture = LoadTexture("../sprites/abominable_mass.png");

    speed = 80.0f;
    velocity = {0.0f, 0.0f};
    knockbackVelocity = {0.0f, 0.0f};
    knockbackResistance = 0.2f;

    width = 28.0f;
    height = 13.0f;
    hitCircle = {pos.x + width / 2.0f, pos.y + height / 2.0f, 8.0f};

    damage = 10.0f;
    health = 10.0f;
    iframes = 1.0f; // invuln for 1 second
    iframeTimer = iframes;
    iframesReady = true;
}

void Enemy::Move(const Player &player) {
    float delta = GetFrameTime();

    Vector2 playerCenter = {player.pos.x + player.width / 2.0f, player.pos.y + player.height / 2.0f};
    Vector2 enemyCenter = {pos.x + width / 2.0f, pos.y + height / 2.0f};
    dir = Vector2Normalize(Vector2Subtract(playerCenter, enemyCenter));

    pos.x += dir.x * speed * delta;
    pos.y += dir.y * speed * delta;

    hitCircle.pos.x = pos.x + width / 2.0f;
    hitCircle.pos.y = pos.y + height / 2.0f;
}

void Enemy::Receive(Vector2 source, Circle damageSource, float knockback, float damage) {
    Vector2 enemyCenter = {hitCircle.pos.x, hitCircle.pos.y};
    Vector2 direction = Vector2Subtract(enemyCenter, source);

    if (Vector2Length(direction) > 0.0f) {
        direction = Vector2Normalize(direction);
    }

    if (iframesReady) {
        knockbackVelocity = Vector2Scale(direction, knockback * (1.0f - knockbackResistance));
        health -= damage;
        iframesReady = false;
        iframeTimer = iframes;
    }
}

void Enemy::Die() {
    for (auto it = enemies.begin(); it != enemies.end();) {
        if ((*it).health <= 0.0f) {
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

    hitCircle.pos.x = pos.x + width / 2.0f;
    hitCircle.pos.y = pos.y + height / 2.0f;
}

void Enemy::Draw() {
    DrawCircleLines(hitCircle.pos.x, hitCircle.pos.y, hitCircle.radius, RED);

    Rectangle srcRect = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
    DrawTextureRec(texture, srcRect, pos, RAYWHITE);
}
