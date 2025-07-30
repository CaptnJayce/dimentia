#pragma once
#include "./AnimationManager.hpp"

#include <raylib.h>
#include <vector>

struct Circle {
    Vector2 pos;
    float radius;
};

class Enemy;
class Player;

class Entity {
public:
    virtual ~Entity() = default;

    virtual void Init() {};
    virtual void ChangeAnimation(AnimState newState) {};

    virtual void Move(Player& player) {};

    virtual void Attack(std::vector<Enemy> &enemies) {};
    virtual void Receive(std::vector<Enemy> &enemies) {};

    virtual void Die(Player &player) {};

    virtual void Update() {};
    virtual void Draw() {};

    // in public since they'll (likely) never have additional logic to change their value
    float width{};
    float height{};

    Camera2D camera{};
    float zoom{};
    float offset{};
    Vector2 target{};

    float expTotal{};

    // getters
    const Circle& GetHitCircle() const { return hitCircle; }
    void SetKnockbackVelocity(const Vector2 vel) { knockbackVelocity = vel; }

    Vector2 GetPos() const { return pos; }
    void SetPos(const float x, const float y) { pos = {x, y}; }

    float GetDamage() const { return damage; }
    float GetHealth() const { return health; }
    float GetSpeed() const { return speed; }
    float GetDashSpeed() const { return dashSpeed; }
    bool GetDashReady() const { return dashReady; }
    int GetAtkCounter() const { return atkCounter; }
    bool GetIfFramesReady() const { return iframesReady; }
    float GetExperience() const { return expTotal; }

    Texture GetTexture() const { return texture; }

protected:
    Texture2D texture{};
    Texture2D atkTexture{};
    Texture2D currentAnim{};

    Vector2 pos{};
    Vector2 cursorPos{};
    Vector2 dir{};
    Vector2 lastDir{};
    Vector2 center{};

    Circle hitCircle{};
    Circle atkCircle{};

    int frameDelay{};
    int frameCounter{};
    int animFrames{};
    int currentAnimFrame{};
    int frameWidth{};
    int frameHeight{};

    float speed{};
    float friction{};

    int level{};
    float expThreshold{};

    float damage{};
    float baseDamage{};
    float bonusDamage{};
    float atkSpeed{};
    float atkCooldown{};
    float atkDuration{};
    float atkActiveTimer{};
    float atkCooldownTimer{};
    int atkCounter{};
    int maxCounter{};
    float spinAngle{};
    float spinSpeed{};
    bool finalAtk{};
    bool atkReady{};

    float knockback{};
    float knockbackResistance{};
    Vector2 knockbackVelocity{};

    float radius{};
    float weaponRadius{};
    float weaponDistance{};

    float dashSpeed{};
    float dashDur{};
    float dashCooldown{};
    float dashTimer{};
    bool dashReady{};

    Vector2 vel{};
    float velocity{};
    float currentVelocity{};
    float maxVelocity{};
    bool moving{};

    float health{};
    float iframes{};
    float iframeTimer{};
    bool iframesReady{};

    AnimationManager animManager;
    AnimState currentAnimState{};
};
