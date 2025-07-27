#pragma once
#include "./AnimationManager.hpp"
#include "./Globals.hpp"

#include <raylib.h>
#include <vector>

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

    virtual void Die() {};

    virtual void Update() {};
    virtual void Draw() {};

    // in public since they'll (likely) never have additional logic to change their value
    float width{};
    float height{};

    // getters
    const Circle& GetHitCircle() const { return hitCircle; }
    void SetKnockbackVelocity(const Vector2 vel) { knockbackVelocity = vel; }
    float GetDamage() const { return damage; }
    float GetHealth() const { return health; }
    Vector2 GetPos() const { return pos; }
    void SetPos(const float x, const float y) { pos = {x, y}; }
    Texture GetTexture() const { return texture; }

protected:
    Texture2D texture{};
    Texture2D atkTexture{};
    Texture2D currentAnim{};

    Vector2 pos{};
    Vector2 cursorPos{};
    Vector2 dir{};
    Vector2 lastDir{};

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

    float damage{};
    float atkSpeed{};
    float atkCooldown{};
    float atkDuration{};
    float atkActiveTimer{};
    float atkCooldownTimer{};
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
