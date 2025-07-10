#include "../include/Enemy.hpp"
#include "../include/Player.hpp"
#include <raylib.h>
#include <vector>

int main() {
    int screenWidth = 1920;
    int screenHeight = 1080;

    SetTargetFPS(120);
    InitWindow(screenWidth, screenHeight, "Kalpa: Chaos");

    Player p;
    p.Init();

    std::vector<Enemy> enemies;

    // TODO: replace with actual spawning logic
    /*
    for (int i = 0; i < 5; i++) {
        Enemy e;
        e.Init();
        e.pos = {static_cast<float>(300 + i * 200), static_cast<float>(200 + i * 100)};
        enemies.push_back(e);
    }
    */

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // Update
        p.Move();
        p.Update(enemies);

        for (auto &enemy : enemies) {
            enemy.Move(p);
            enemy.Update();
        }

        // Draw
        BeginDrawing();
        BeginMode2D(p.camera);
        ClearBackground({25, 23, 36, 255});

        p.Draw();
        for (auto &enemy : enemies) {
            enemy.Draw();
        }

        EndMode2D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
