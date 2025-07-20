#include "../include/Enemy.hpp"
#include "../include/GUI.hpp"
#include "../include/Player.hpp"
#include "../include/SpatialGrid.hpp"
#include <raylib.h>

int main() {
    int screenWidth = 1920;
    int screenHeight = 1080;

    SetTargetFPS(120);
    InitWindow(screenWidth, screenHeight, "Kalpa: Chaos");

    Player p;
    p.Init();

    UI ui(p);

    float cellSize = 25.0f;
    SpatialGrid grid(screenWidth, screenHeight, cellSize);

    for (int i = 0; i < 5; i++) {
        Enemy e;
        e.Init();
        e.pos = {static_cast<float>(300 + i * 200), static_cast<float>(200 + i * 100)};
        enemies.push_back(e);
        grid.Insert(&enemies.back());
    }

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // Update
        p.Move();

        grid.Clear();
        for (Enemy &enemy : enemies) {
            enemy.Move(p);
            enemy.Update(grid);
            grid.Insert(&enemy);
        }

        p.Update(enemies);

        if (IsKeyPressed(KEY_ONE)) {
            ui.LoadScene(UI::MainMenu);
        }
        if (IsKeyPressed(KEY_TWO)) {
            ui.LoadScene(UI::Game);
        }
        ui.Update();

        for (auto it = enemies.begin(); it != enemies.end();) {
            if (it->health <= 0.0f) {
                grid.Remove(&(*it));
                UnloadTexture(it->texture);
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }

        // Draw
        BeginDrawing();
        BeginMode2D(p.camera);
        ClearBackground({25, 23, 36, 255});

        p.Draw();
        for (Enemy &enemy : enemies) {
            enemy.Draw();
        }

        grid.Draw();

        EndMode2D();
        ui.Draw();
        EndDrawing();
    }

    for (Enemy &enemy : enemies) {
        UnloadTexture(enemy.texture);
    }
    CloseWindow();
    return 0;
}
