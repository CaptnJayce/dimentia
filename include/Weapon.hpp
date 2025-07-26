#pragma once
#include "./AnimationManager.hpp"
#include "./Globals.hpp"

#include <vector>

class Enemy;

class Weapon {
public:
    void Attack(std::vector<Enemy> &enemies);

    float damage;
    float knockback;

    float width;
    float height;

    AnimState currentAnimState;
private:
    float m_atkSpeed{};
    float m_atkCooldown{};
    float m_atkDuration{};
    float m_atkActiveTimer{};
    float m_atkCooldownTimer{};
    bool m_atkReady{};

    Texture2D m_atkTexture{};
    Texture2D m_currentAnim{};
    float m_weaponRadius{};
    float m_weaponDistance{};
    Circle m_atkCircle{};
};