#include "../include/Enemy.hpp"
#include "../include/Player.hpp"
#include <raylib.h>
#include <raymath.h>

void Enemy::Init() {
    pos = {200, 200};
    dir = {0, 0};

    texture = LoadTexture("../sprites/abominable_mass.png");

    speed = 80.0;

    width = 10;
    height = 10;
}

void Enemy::Move(const Player &player) {
    float delta = GetFrameTime();

    // good nuff
    dir = Vector2Normalize(player.pos - pos);
    pos.x += dir.x * speed * delta;
    pos.y += dir.y * speed * delta;
}

void Enemy::Draw() { DrawTextureV(texture, pos, RAYWHITE); }
