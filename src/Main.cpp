#include "../include/Enemy.hpp"
#include "../include/GUI.hpp"
#include "../include/Globals.hpp"
#include "../include/Player.hpp"
#include "../include/SpatialGrid.hpp"
#include <raylib.h>

int main() {
    int screenWidth = 1920;
    int screenHeight = 1080;

    SetTargetFPS(120);
    InitWindow(screenWidth, screenHeight, "Kalpa: Chaos");

    InitTextures();

    Player p;
    p.Init();

    UI ui(p);

    float cellSize = 25.0f;
    SpatialGrid grid(screenWidth, screenHeight, cellSize);

    for (int i = 0; i < 10; i++) {
        Enemy e;
        e.Init();
        e.SetPos(
            static_cast<float>(GetRandomValue(-100, 100)),
            static_cast<float>(GetRandomValue(-100, 100))
        );
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
            enemy.Update();
            grid.Insert(&enemy);
        }

        p.Update();

        if (IsKeyPressed(KEY_ONE)) {
            ui.LoadScene(UI::MainMenu);
        }
        if (IsKeyPressed(KEY_TWO)) {
            ui.LoadScene(UI::Game);
        }
        ui.Update();

        for (auto it = enemies.begin(); it != enemies.end();) {
            if (it->GetHealth()<= 0.0f) {
                grid.Remove(&(*it));
                UnloadTexture(it->GetTexture());
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
        UnloadTexture(enemy.GetTexture());
    }
    CloseWindow();
    return 0;
}
