#include "../include/GUI.hpp"
#include "../include/Globals.hpp"

#include <raylib.h>

// TODO: change Game to MainMenu when introduced
// Default Menu
UI::UI(Player& player, Weapon& weapon) : m_currentScene(MainMenu), m_previousScene(Game), m_player(player), m_weapon(weapon) {
    InitMainMenuUI();
    InitGameUI();
}

UI::~UI() { UnloadCurrentScene(); }

void UI::Init() {
    // TODO: global UI
}

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
}

void UI::Draw() {
    if (m_drawFunctions.find(m_currentScene) != m_drawFunctions.end()) {
        m_drawFunctions[m_currentScene]();
    }
}

void UI::InitMainMenuUI() {
    m_drawFunctions[MainMenu] = [this]() {
        const char *title = "Main Menu";
        constexpr int fontSize = 40;
        const int textWidth = MeasureText(title, fontSize);

        DrawText(title, (screenWidth - textWidth) / 2, screenHeight * 0.2f, fontSize, WHITE);

        const char *prompt = "Press [ENTER] to Start";
        constexpr int promptFontSize = 20;
        const int promptWidth = MeasureText(prompt, promptFontSize);
        DrawText(prompt,(screenWidth - promptWidth) / 2,screenHeight * 0.5f, promptFontSize, LIGHTGRAY);
    };

    m_updateFunctions[MainMenu] = [this]() {
        if (IsKeyPressed(KEY_ENTER)) {
            LoadScene(Game);
        }
    };
}

void UI::InitGameUI() {
    m_drawFunctions[Game] = [this]() {
        DrawText("Game UI", 10, 10, 20, WHITE);
        DrawFPS(10, 30);
        DrawText(TextFormat("Health %f", health), 10, 50, 20, RED);
        DrawText(TextFormat("Damage %f", damage), 10, 70, 20, ORANGE);
        DrawText(TextFormat("Speed %f", speed), 10, 90, 20, YELLOW);
        DrawText(TextFormat("Dash %f", dashSpeed), 10, 110, 20, GREEN);
        DrawText(TextFormat("Dash Ready %b", dashReady), 10, 130, 20, BLUE);
        DrawText(TextFormat("Counter %d", atkCounter), 10, 150, 20, VIOLET);
        DrawText(TextFormat("IFrames Ready %b", iframesReady), 10, 170, 20, RED);
        DrawText(TextFormat("Total Exp %f", expTotal), 10, 190, 20, ORANGE);
        DrawText(TextFormat("Level %d", level), 10, 210, 20, YELLOW);
        DrawText(TextFormat("Threshold %f", threshold), 10, 230, 20, GREEN);
    };

    m_updateFunctions[Game] = []() {};
}
