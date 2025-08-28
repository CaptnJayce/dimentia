#include "Weapon.hpp"
#include "Enemy.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "raymath.h"
#include <raylib.h>

void Weapon::Init() {
  baseDamage = 5.0f;
  damage = baseDamage;
  bonusDamage = 10.0f;
  atkSpeed = 100.0f;
  atkCooldown = 0.1f;
  atkDuration = 0.3f;
  atkActiveTimer = 0.0f;
  atkCooldownTimer = 0.0f;
  atkReady = true;
  knockback = 250.0f;
  weaponRadius = 8.0f;
  weaponDistance = 30.0f;
  atkCounter = 0;
  maxCounter = 3;
  atkTexture = textures.crescentSlashTexture;
}

void Weapon::Attack(std::vector<Enemy> &enemies, Player &player) {
  if (atkActiveTimer <= 0.0f) {
    if (finalAtk) {
      damage = baseDamage;
      finalAtk = false;
    }
    return;
  }

  Vector2 weaponPosition;

  if (finalAtk) {
    spinSpeed = 40.0f;
    spinAngle += spinSpeed * GetFrameTime();
    weaponPosition = {pos.x + cosf(spinAngle) * weaponDistance, pos.y + sinf(spinAngle) * weaponDistance};
  } else {
    const Vector2 direction = Vector2Subtract(cursorPos, pos);
    const Vector2 normalized = Vector2Normalize(direction);
    weaponPosition = {pos.x + normalized.x * weaponDistance, pos.y + normalized.y * weaponDistance};
  }

  atkCircle.pos = weaponPosition;

  for (int i = 0; i < enemies.size();) {
    if (Enemy &enemy = enemies[i]; CheckCollisionCircles(weaponPosition, atkCircle.radius, {enemy.GetHitCircle().pos.x, enemy.GetHitCircle().pos.y}, enemy.GetHitCircle().radius)) {
      enemy.Receive(pos, knockback, damage, player);
    }
    i++;
  }

  player.LevelUp();
}

void Weapon::Draw() {
  float rotation;
  Vector2 weaponPosition;

  if (finalAtk) {
    weaponPosition = atkCircle.pos;
    rotation = spinAngle * RAD2DEG;
  } else {
    const Vector2 direction = Vector2Subtract(cursorPos, pos);
    rotation = atan2f(direction.y, direction.x) * RAD2DEG;
    weaponPosition = atkCircle.pos;
  }

  if (atkActiveTimer > 0.0f) {
    DrawCircleLines(weaponPosition.x, weaponPosition.y, atkCircle.radius, RED);

    const Vector2 txtOrigin = {static_cast<float>(atkTexture.width) / 2.0f, static_cast<float>(atkTexture.height) / 2.0f};
    const Rectangle destRec = {weaponPosition.x, weaponPosition.y, static_cast<float>(atkTexture.width), static_cast<float>(atkTexture.height)};
    DrawTexturePro(atkTexture, Rectangle{0.0f, 0.0f, static_cast<float>(atkTexture.width), static_cast<float>(atkTexture.height)}, destRec, txtOrigin, rotation, WHITE);
  }

  const bool flipSprite = (lastDir.x < 0.0f);
  animManager.Draw(pos, camera, flipSprite);
}

void Weapon::Update(Player &player) {
  const float delta = GetFrameTime();
  cursorPos = GetScreenToWorld2D(GetMousePosition(), player.camera);

  const Vector2 playerCenter = {player.GetPos().x + player.width / 2.0f, player.GetPos().y + player.height / 2.0f};
  pos = playerCenter;

  // needed for debug
  atkCircle = {pos.x, pos.y, weaponRadius};

  atkActiveTimer -= delta;
  atkCooldownTimer -= delta;

  if (atkCooldownTimer <= 0.0f) {
    atkReady = true;
  }

  // apply cooldown at end of combo
  if (atkCounter == maxCounter) {
    atkCooldownTimer += 1.0f;
    atkCounter = 0;
  }

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && atkReady) {
    atkCounter += 1;
    atkActiveTimer = atkDuration;
    atkCooldownTimer = atkDuration + atkCooldown;
    atkReady = false;

    if (atkCounter == 3) {
      finalAtk = true;
      spinAngle = -PI / 2;
      damage = baseDamage + bonusDamage;
    }
  }

  Attack(enemies, player);
}
