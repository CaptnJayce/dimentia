#pragma once
#include <raylib.h>

namespace Collision {
inline bool CheckRectangles(const Rectangle &rect1, const Rectangle &rect2) {
    bool isColliding = CheckCollisionRecs(rect1, rect2);
    return isColliding;
}
} // namespace Collision
// THE ABOVE COMMENT WONT GO AWAY I HAVE NO IDEA WHY
