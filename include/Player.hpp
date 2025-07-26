#pragma once
#include "./AnimationManager.hpp"
#include "./Globals.hpp"

#include <raylib.h>
#include <vector>

class Enemy;

class Player {
  public:
    void Init();
    void ChangeAnimation(AnimState newState);
    void Move();
    void Receive(std::vector<Enemy> &enemies);
    void Update(std::vector<Enemy> &enemies);
    void Draw();

    Vector2 pos;
    Circle hitCircle;

    float health;

    float width;
    float height;

    Camera2D camera;

    AnimState currentAnimState;

  private:
    Vector2 m_dir{};
    Vector2 m_lastDir{};
    float m_speed{};
    float m_friction{};
    float m_velocity{};
    float m_currentVelocity{};
    float m_maxVelocity{};
    bool m_moving{};

    float m_dashSpeed{};
    float m_dashDur{};
    float m_dashCooldown{};
    float m_dashTimer{};
    bool m_dashReady{};

    Texture2D m_idle{};
    Texture2D m_run{};
    Texture2D m_currentAnim{};
    int m_frameDelay{};
    int m_frameCounter{};
    int m_animFrames{};
    int m_currentAnimFrame{};
    int m_frameWidth{};
    int m_frameHeight{};
    AnimationManager m_animManager;

    Vector2 m_cursorPos{};

    float m_iframes{};
    float m_iframeTimer{};
    bool m_iframesReady{};

    float m_zoom{};
    float m_offset{};
    Vector2 m_target{};

    float m_radius{};
};