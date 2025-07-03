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

    health = 10.0;
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

void Enemy::Update() {}

void Enemy::Draw() {
    // Rectangle rect = {0, 0, (float)texture.width, (float)texture.height};

    Rectangle rect = {0, 0, (float)texture.width, (float)texture.height};
    DrawTextureRec(texture, rect, pos, RAYWHITE);
}
