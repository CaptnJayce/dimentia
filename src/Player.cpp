#include "../include/Player.hpp"
#include "../include/AnimationManager.hpp"
#include "../include/Enemy.hpp"
#include <algorithm>
#include <raylib.h>
#include <raymath.h>

void Player::Init() {
    m_animManager.AddAnimation(AnimState::IDLE, "../sprites/s_HuskOneIdle.png", 19, 0.1f, true);
    m_animManager.AddAnimation(AnimState::RUN, "../sprites/s_HuskOneRun.png", 6, 0.1f, true);
    m_animManager.AddAnimation(AnimState::ATTACK, "../sprites/s_CrescentSlashOne.png", 5, 0.1f, false);
    currentAnimState = AnimState::IDLE;

    pos = {100.0f, 100.0f};
    m_dir = {0.0f, 0.0f};
    m_lastDir = {0.0f, 0.0f};
    m_cursorPos = {0.0f, 0.0f};

    m_speed = 10.0f;
    m_dashSpeed = 300.0f;
    m_dashDur = 0.2f;
    m_dashCooldown = 3.0f;
    m_dashTimer = 0.0f;
    m_dashReady = true;
    m_friction = 4000.0f;
    m_velocity = 200.0f;
    m_currentVelocity = 0.0f;
    m_maxVelocity = 100.0f;
    m_moving = false;

    damage = 5.0f;
    m_atkSpeed = 100.0f;
    m_atkCooldown = 0.5f;
    m_atkDuration = 0.5f;
    m_atkActiveTimer = 0.0f;
    m_atkCooldownTimer = 0.0f;
    m_atkReady = true;
    knockback = 500.0f;
    m_weaponRadius = 8.0f;
    m_weaponDistance = 15.0f;
    m_atkTexture = textures.huskOneWeaponTexture;

    health = 100.0f;
    m_iframes = 1.0f;
    m_iframeTimer = m_iframes;
    m_iframesReady = true;

    width = 10.0f;
    height = 22.0f;

    const Vector2 center = {pos.x + width / 2.0f, pos.y + height / 2.0f};
    hitCircle = {center.x, center.y, 8.0f};
    m_atkCircle = {center.x, center.y, m_weaponRadius};

    camera = {0};
    camera.target = pos;
    camera.offset = {1920.0f / 2.0f, 1080.0f / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
}

void Player::Move() {
    const float delta = GetFrameTime();
    m_dashTimer -= delta;

    if (IsKeyPressed(KEY_LEFT_SHIFT) && m_dashReady) {
        m_dashTimer = m_dashDur;
        m_dashReady = false;
    }

    if (m_dashTimer > 0.0f) {
        pos.x += m_lastDir.x * m_dashSpeed * delta;
        pos.y += m_lastDir.y * m_dashSpeed * delta;
    } else {
        if (m_dashTimer <= -m_dashCooldown) {
            m_dashReady = true;
        }

        m_dir = {0.0f, 0.0f};
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
            m_dir.y -= 1.0f;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
            m_dir.y += 1.0f;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            m_dir.x -= 1.0f;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            m_dir.x += 1.0f;

        if (m_dir.x != 0.0f || m_dir.y != 0.0f) {
            m_dir = Vector2Normalize(m_dir);
            m_moving = true;
            m_lastDir = m_dir;
            m_currentVelocity += m_speed * m_velocity * delta;
            m_currentVelocity = std::min(m_currentVelocity, m_maxVelocity);
        } else {
            m_moving = false;
            m_currentVelocity -= m_friction * delta;
            m_currentVelocity = std::max(0.0f, m_currentVelocity);
        }

        pos.x += m_lastDir.x * m_currentVelocity * delta;
        pos.y += m_lastDir.y * m_currentVelocity * delta;

        m_currentVelocity = std::min(m_currentVelocity, m_maxVelocity);
    }

    hitCircle.pos.x = pos.x + width / 2.0f;
    hitCircle.pos.y = pos.y + height / 2.0f;

    camera.target = Vector2Lerp(camera.target, pos, 20.0f * delta);
}

void Player::Attack(std::vector<Enemy> &enemies) {
    if (m_atkActiveTimer <= 0.0f) {
        return;
    }

    const Vector2 playerCenter = {pos.x + width / 2.0f, pos.y + height / 2.0f};
    const Vector2 direction = Vector2Subtract(m_cursorPos, playerCenter);
    const Vector2 normalized = Vector2Normalize(direction);

    const Vector2 weaponPosition = {
        playerCenter.x + normalized.x * m_weaponDistance,
        playerCenter.y + normalized.y * m_weaponDistance
    };

    m_atkCircle.pos.x = weaponPosition.x;
    m_atkCircle.pos.y = weaponPosition.y;

    for (int i = 0; i < enemies.size();) {
        Enemy &enemy = enemies[i];
        if (CheckCollisionCircles(
            weaponPosition, m_atkCircle.radius,
            {enemy.hitCircle.pos.x, enemy.hitCircle.pos.y},
            enemy.hitCircle.radius
        )) {
            enemy.Receive(pos, m_atkCircle, knockback, damage);
            if (enemy.health <= 0.0f) {
                enemies.erase(enemies.begin() + i);
                continue;
            }
        }
        i++;
    }
}

void Player::Receive(std::vector<Enemy> &enemies) {
    const float delta = GetFrameTime();

    if (!m_iframesReady) {
        m_iframeTimer -= delta;
        if (m_iframeTimer <= 0.0f) {
            m_iframesReady = true;
        }
    }

    if (m_iframesReady) {
        bool tookDamage = false;
        for (const Enemy &enemy : enemies) {
            if (CheckCollisionCircles(
                {hitCircle.pos.x, hitCircle.pos.y}, hitCircle.radius,
                {enemy.hitCircle.pos.x, enemy.hitCircle.pos.y}, enemy.hitCircle.radius
            )) {
                health -= enemy.damage;
                tookDamage = true;
                break;
            }
        }

        if (tookDamage) {
            m_iframesReady = false;
            m_iframeTimer = m_iframes;

            // knockback all enemies on hit
            for (Enemy &enemy : enemies) {
                const Vector2 enemyCenter = {enemy.hitCircle.pos.x, enemy.hitCircle.pos.y};
                const Vector2 playerCenter = {hitCircle.pos.x, hitCircle.pos.y};
                Vector2 direction = Vector2Subtract(enemyCenter, playerCenter);

                if (Vector2Length(direction) > 0.0f) {
                    direction = Vector2Normalize(direction);
                }

                enemy.knockbackVelocity = Vector2Scale(direction, knockback * 0.5f);
            }
        }
    }
}

void Player::Update(std::vector<Enemy> &enemies) {
    const float delta = GetFrameTime();

    m_animManager.Update();

    m_cursorPos = GetScreenToWorld2D(GetMousePosition(), camera);

    m_atkActiveTimer -= delta;
    m_atkCooldownTimer -= delta;

    if (m_atkCooldownTimer <= 0.0f) {
        m_atkReady = true;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && m_atkReady) {
        m_atkActiveTimer = m_atkDuration;
        m_atkCooldownTimer = m_atkDuration + m_atkCooldown;
        m_atkReady = false;
        ChangeAnimation(AnimState::ATTACK);
    }

    if (m_atkActiveTimer <= 0.0f) {
        if (m_moving) {
            ChangeAnimation(AnimState::RUN);
        } else {
            ChangeAnimation(AnimState::IDLE);
        }
    }

    Attack(enemies);
    Receive(enemies);
}

void Player::Draw() {
    DrawCircleV(m_cursorPos, 5.0f, RED);

    const Vector2 direction = Vector2Subtract(m_cursorPos, pos);
    const float rotation = atan2f(direction.y, direction.x) * RAD2DEG;

    if (m_atkActiveTimer > 0.0f) {
        const Vector2 txtOrigin = {
            static_cast<float>(m_atkTexture.width) / 2.0f,
            static_cast<float>(m_atkTexture.height) / 2.0f
        };
        const Rectangle destRec = {
            m_atkCircle.pos.x, m_atkCircle.pos.y,
            static_cast<float>(m_atkTexture.width),
            static_cast<float>(m_atkTexture.height)
        };
        DrawTexturePro(
            m_atkTexture,
            Rectangle{0.0f, 0.0f, static_cast<float>(m_atkTexture.width), static_cast<float>(m_atkTexture.height)},
            destRec,
            txtOrigin,
            rotation,
            WHITE
        );
    }

    const bool flipSprite = (m_lastDir.x < 0.0f);
    m_animManager.Draw(pos, flipSprite);
}

void Player::ChangeAnimation(const AnimState newState) {
    if (currentAnimState != newState) {
        currentAnimState = newState;
        m_animManager.Play(newState);
    }
}
