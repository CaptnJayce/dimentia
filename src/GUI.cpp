#include "../include/GUI.hpp"
#include <raylib.h>

// TODO: change Game to MainMenu when introduced
UI::UI() : m_currentScene(Game), m_previousScene(Game) {
    InitMainMenuUI();
    InitGameUI();
}

UI::~UI() { UnloadCurrentScene(); }

void UI::Init() {
    // TODO: global UI
}

void UI::LoadScene(Scene newScene) {
    if (m_currentScene == newScene) {
        return;
    }

    UnloadCurrentScene();
    m_previousScene = m_currentScene;
    m_currentScene = newScene;
}

void UI::UnloadCurrentScene() {
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
    m_drawFunctions[MainMenu] = []() { DrawText("MainMenu UI Test", 150, 10, 20, WHITE); };

    m_updateFunctions[MainMenu] = []() {};
}

void UI::InitGameUI() {
    m_drawFunctions[Game] = []() { DrawText("Game UI Test", 150, 10, 20, WHITE); };

    m_updateFunctions[Game] = []() {};
}
