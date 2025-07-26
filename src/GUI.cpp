#include "../include/GUI.hpp"
#include "../include/Player.hpp"
#include <raylib.h>

// TODO: change Game to MainMenu when introduced
// Default Menu
UI::UI(Player &player) : m_currentScene(Game), m_previousScene(Game), m_player(player) {
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
}

void UI::Draw() {
    if (m_drawFunctions.find(m_currentScene) != m_drawFunctions.end()) {
        m_drawFunctions[m_currentScene]();
    }
}

void UI::InitMainMenuUI() {
    m_drawFunctions[MainMenu] = []() { DrawText("MainMenu UI Test", 10, 10, 20, WHITE); };

    m_updateFunctions[MainMenu] = []() {};
}

void UI::InitGameUI() {
    m_drawFunctions[Game] = [this]() {
        DrawText("Game UI", 10, 10, 20, WHITE);
        DrawFPS(10, 30);
        DrawText(TextFormat("Health %f", m_player.health), 10, 50, 20, RED);
        DrawText(TextFormat("Damage %f", m_player.damage), 10, 70, 20, ORANGE);
    };

    m_updateFunctions[Game] = []() {};
}
