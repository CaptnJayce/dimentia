#include "../include/Player.hpp"
#include <raylib.h>

int main() {
    int screenWidth = 1920;
    int screenHeight = 1080;

    SetTargetFPS(120);
    InitWindow(screenWidth, screenHeight, "Kalpa: Chaos");

    Player p;
    p.Init();

    while (!WindowShouldClose()) {
        // Update
        p.Move();

        // Draw
        BeginDrawing();
        BeginMode2D(p.camera);
        ClearBackground(BLACK);

        DrawRectangle(p.pos.x, p.pos.y, p.width, p.height, p.colour);

        EndMode2D();
        EndDrawing();
    }

    DrawFPS(10, 10);
    return 0;
}
