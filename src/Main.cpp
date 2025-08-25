#include "../include/GUI.hpp"
#include "../include/Globals.hpp"
#include "../include/Player.hpp"
#include "../include/SpatialGrid.hpp"
#include <raylib.h>

int main() {
  SetTargetFPS(120);
  InitWindow(screenWidth, screenHeight, "Dimentia");
  SetExitKey(KEY_NULL);

  InitTextures();

  Player p;
  Weapon w;
  p.Init();
  w.Init();

  UI ui(p, w);

  // TODO: sophisticated enemy spawning system
  for (int i = 0; i < 100; i++) {
    Enemy e;
    e.Init();
    e.SetPos(static_cast<float>(GetRandomValue(0, 1000)),
             static_cast<float>(GetRandomValue(0, 1000)));
    enemies.push_back(e);
    grid.Insert(&enemies.back());
  }

  while (!WindowShouldClose()) {
    // TODO: move UI logic to UI::Update function
    switch (ui.GetCurrentScene()) {
    case UI::MainMenu:
      if (IsKeyPressed(KEY_ENTER)) {
        ui.LoadScene(UI::Game);
      }
      break;

    case UI::Game:
      if (IsKeyPressed(KEY_ESCAPE)) {
        ui.LoadScene(UI::Pause);
      }
      break;

    case UI::Settings:
      if (IsKeyPressed(KEY_ESCAPE)) {
        ui.LoadScene(UI::MainMenu);
      }
      break;

    case UI::Pause:
      if (IsKeyPressed(KEY_ESCAPE)) {
        ui.LoadScene(UI::Game);
      }
      break;

    default:
      break;
    }

    ui.Update();

    if (ui.GetCurrentScene() == UI::Game) {
      p.Move();

      grid.Clear();
      for (Enemy &enemy : enemies) {
        enemy.Move(p);
        enemy.Update();
        grid.Insert(&enemy);
      }

      p.Update();
      w.Update(p);

      for (auto it = enemies.begin(); it != enemies.end();) {
        if (it->GetHealth() <= 0.0f) {
          grid.Remove(&(*it));
          UnloadTexture(it->GetTexture());
          it = enemies.erase(it);
        } else {
          ++it;
        }
      }
    }

    BeginDrawing();

    if (ui.GetCurrentScene() == UI::Game || ui.GetCurrentScene() == UI::Pause) {
      BeginMode2D(p.camera);
      ClearBackground({25, 23, 36, 255});

      p.Draw();
      w.Draw();
      for (Enemy &enemy : enemies) {
        enemy.Draw();
      }

      // grid.Draw();
      EndMode2D();

      // dim screen if paused
      if (ui.GetCurrentScene() == UI::Pause) {
        DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 180});
      }
    } else {
      ClearBackground(BLACK);
    }

    ui.Draw();

    EndDrawing();
  }

  // TODO: Move into helper function
  for (Enemy &enemy : enemies) {
    UnloadTexture(enemy.GetTexture());
  }
  CloseWindow();
  return 0;
}
