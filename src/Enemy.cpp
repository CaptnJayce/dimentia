#include "../include/Enemy.hpp"
#include "../include/Player.hpp"
#include <raylib.h>
#include <raymath.h>

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

void Enemy::Knockback(Vector2 source, float knockback) {
    Vector2 direction = Vector2Subtract(pos, source);
    if (Vector2Length(direction) > 0) {
        direction = Vector2Normalize(direction);
    }

    knockbackVelocity = Vector2Scale(direction, knockback);
}

void Enemy::Update() {
    float delta = GetFrameTime();

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
