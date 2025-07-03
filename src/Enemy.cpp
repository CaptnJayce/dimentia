#include "../include/Enemy.hpp"
#include <raylib.h>

void Enemy::Init() {
    pos = {200, 200};

    colour = RED;

    speed = 10.0;

    width = 10;
    height = 10;
}

void Enemy::Move() {
    // TODO: Move enemy towards player
}

void Enemy::Draw() { DrawRectangle(pos.x, pos.y, width, height, colour); }
