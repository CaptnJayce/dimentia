#pragma once
#include <raylib.h>
#include <vector>

// TODO: projectile vector

class Enemy;

namespace Utils {
    inline bool CheckRectangles(const Rectangle &rect1, const Rectangle &rect2) {
        return CheckCollisionRecs(rect1, rect2);
    }

    inline std::vector<Enemy> enemies;
}
