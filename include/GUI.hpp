#pragma once
#include "./Player.hpp"
#include <functional>
#include <unordered_map>

class UI {
  public:
    UI(Player &player);
    ~UI();

    enum Scene { MainMenu, Game, Settings, Pause };

    void LoadScene(Scene newScene);
    Scene GetCurrentScene() const { return m_currentScene; }

    void Init();
    void Update();
    void Draw();

  private:
    Scene m_currentScene;
    Scene m_previousScene;

    Player &m_player;

    std::unordered_map<Scene, std::function<void()>> m_drawFunctions;
    std::unordered_map<Scene, std::function<void()>> m_updateFunctions;

    void InitMainMenuUI();
    void InitGameUI();

    void UnloadCurrentScene();
};
