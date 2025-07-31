#pragma once
#include "./Player.hpp"
#include <functional>
#include <unordered_map>

class UI {
  public:
    UI(Player& player, Weapon& weapon);
    ~UI();

    enum Scene { MainMenu, Game, Settings, Pause };

    void LoadScene(Scene newScene);
    Scene GetCurrentScene() const { return m_currentScene; }

    float health;
    float damage;
    float speed;
    int atkCounter;
    float dashSpeed;
    bool dashReady;
    bool iframesReady;
    float expTotal;
    int level;
    double threshold;

    static void Init();
    void Update();
    void Draw();

  private:
    Scene m_currentScene;
    Scene m_previousScene;

    Player& m_player;
    Weapon& m_weapon;

    std::unordered_map<Scene, std::function<void()>> m_drawFunctions;
    std::unordered_map<Scene, std::function<void()>> m_updateFunctions;

    void InitMainMenuUI();
    void InitGameUI();

    void UnloadCurrentScene() const;
};
