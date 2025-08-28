#include "GUI.hpp"
#include "Enemy.hpp"
#include "Entity.hpp"
#include "Globals.hpp"

#define RAYGUI_IMPLEMENTATION // fuck you
#include "../../raygui.h"
#include <raylib.h>

/* TODO:
 * Custom font
 * Controls for font size
 * Hide player stats in pause menu
 */

// Default Menu
UI::UI(Player &player, Weapon &weapon) : m_currentScene(MainMenu), m_previousScene(Game), m_player(player), m_weapon(weapon) {
  InitMainMenuUI();
  InitGameUI();
  InitPauseUI();
  InitSettingsUI();
}

UI::~UI() { UnloadCurrentScene(); }

void UI::LoadScene(const Scene newScene) {
  if (m_currentScene == newScene) {
    return;
  }

  UnloadCurrentScene();
  m_previousScene = m_currentScene;
  m_currentScene = newScene;
}

void UI::UnloadCurrentScene() const {
  switch (m_currentScene) {
  case MainMenu:
    break;
  case Game:
    break;
  case Settings:
    break;
  case Pause:
    break;
  }
}

void UI::Update() {
  if (m_updateFunctions.find(m_currentScene) != m_updateFunctions.end()) {
    m_updateFunctions[m_currentScene]();
  }

  health = m_player.GetHealth();
  damage = m_player.GetDamage();
  speed = m_player.GetSpeed();
  dashSpeed = m_player.GetDashSpeed();
  dashReady = m_player.GetDashReady();
  atkCounter = m_weapon.GetAtkCounter();
  iframesReady = m_player.GetIfFramesReady();
  expTotal = m_player.GetExperience();
  level = m_player.GetLevel();
  threshold = m_player.GetThreshold();

  switch (GetCurrentScene()) {
  case UI::MainMenu:
    if (IsKeyPressed(KEY_ENTER)) {
      LoadScene(UI::Game);
    }
    break;

  case UI::Game:
    if (IsKeyPressed(KEY_ESCAPE)) {
      LoadScene(UI::Pause);
    }
    break;

  case UI::Settings:
    if (IsKeyPressed(KEY_ESCAPE)) {
      LoadScene(UI::MainMenu);
    }
    break;

  case UI::Pause:
    if (IsKeyPressed(KEY_ESCAPE)) {
      LoadScene(UI::Game);
    }
    break;

  default:
    break;
  }
}

void UI::Draw() {
  if (m_drawFunctions.find(m_currentScene) != m_drawFunctions.end()) {
    m_drawFunctions[m_currentScene]();
  }
}

void UI::InitMainMenuUI() {
  m_drawFunctions[MainMenu] = [this] {
    const auto title = "Kalpa: Dimentia";
    constexpr int fontSize = 40;
    const int textWidth = MeasureText(title, fontSize);
    const int x = (screenWidth - textWidth) / 2;
    const int y = static_cast<int>(screenHeight * 0.2f);

    DrawText(title, x, y, fontSize, WHITE);

    Rectangle buttonRect = {screenWidth / 2.0f - 100.0f, screenHeight * 0.5f, 200.0f, 40.0f};

    if (GuiButton(buttonRect, "Start Game")) {
      LoadScene(Game);
    }

    buttonRect.y += 100.0f;
    if (GuiButton(buttonRect, "Settings")) {
      LoadScene(Settings);
    }

    buttonRect.y += 100.0f;
    if (GuiButton(buttonRect, "Quit")) {
      shouldClose = true;
    }
  };
}

void UI::InitPauseUI() {
  m_drawFunctions[Pause] = [this] {
    const auto title = "Game Paused";
    constexpr int fontSize = 40;
    const int textWidth = MeasureText(title, fontSize);
    const int x = static_cast<int>((static_cast<float>(screenWidth) - static_cast<float>(textWidth)) / 2.0f);
    const int y = static_cast<int>(static_cast<float>(screenHeight) * 0.2f);

    const auto hint = "Progress will be saved until game is closed";
    const int hintWidth = MeasureText(hint, fontSize / 2);
    const int hintX = static_cast<int>((static_cast<float>(screenWidth) - static_cast<float>(hintWidth)) / 2.0f);
    const int hintY = static_cast<int>(static_cast<float>(screenHeight) * 0.8f);

    DrawText(title, x, y, fontSize, WHITE);

    Rectangle buttonRect = {static_cast<float>(screenWidth) / 2.0f - 100.0f, static_cast<float>(screenHeight) * 0.5f, 200.0f, 40.0f};

    if (GuiButton(buttonRect, "Resume")) {
      LoadScene(Game);
    }

    buttonRect.y += 100.0f;
    if (GuiButton(buttonRect, "Quit")) {
      LoadScene(MainMenu);
    }
    DrawText(hint, hintX, hintY, fontSize / 2, WHITE);
  };
}

void UI::InitSettingsUI() {
  m_drawFunctions[Settings] = [this] {
    const char *title = "Settings";
    constexpr int fontSize = 40;
    const int textWidth = MeasureText(title, fontSize);
    const int x = (screenWidth - textWidth) / 2;
    const int y = static_cast<int>(screenHeight * 0.2f);
    Rectangle sliderRect = {static_cast<float>(screenWidth) / 2.0f - 100.0f, static_cast<float>(screenHeight) * 0.5f, 200.0f, 40.0f};

    DrawText(title, x, y, fontSize, WHITE);

    constexpr float step = 1.0f;

    GuiSlider(sliderRect, "Volume Slider", NULL, &volumeLevel, 0.0f, 100.0f);
    volumeLevel = roundf(volumeLevel / step) * step;
    sliderRect.y += 100.0f;

    GuiSlider(sliderRect, "Camera Zoom", NULL, &zoomLevel, 1.0f, 10.0f);
    zoomLevel = roundf(zoomLevel / step) * step;
  };
}

void UI::InitGameUI() {
  m_drawFunctions[Game] = [this] {
    DrawText("Press ESC to Pause", 10, 10, 20, WHITE);
    DrawFPS(10, 30);
    DrawText(TextFormat("Health %f", health), 10, 50, 20, RED);
    DrawText(TextFormat("Damage %f", damage), 10, 70, 20, ORANGE);
    DrawText(TextFormat("Speed %f", speed), 10, 90, 20, YELLOW);
    DrawText(TextFormat("Dash %f", dashSpeed), 10, 110, 20, GREEN);
    DrawText(TextFormat("Dash Ready %d", static_cast<int>(dashReady)), 10, 130, 20, BLUE);
    DrawText(TextFormat("Counter %d", atkCounter), 10, 150, 20, VIOLET);
    DrawText(TextFormat("IFrames Ready %d", static_cast<int>(iframesReady)), 10, 170, 20, RED);
    DrawText(TextFormat("Total Exp %f", expTotal), 10, 190, 20, ORANGE);
    DrawText(TextFormat("Level %d", level), 10, 210, 20, YELLOW);
    DrawText(TextFormat("Threshold %f", threshold), 10, 230, 20, GREEN);
  };
}
