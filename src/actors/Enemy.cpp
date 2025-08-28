#include "Enemy.hpp"
#include "Globals.hpp"
#include "Player.hpp"
#include "SpatialGrid.hpp"

#include <raylib.h>
#include <raymath.h>

std::vector<Enemy> enemies;

void Enemy::Init() {
  pos = {200.0f, 200.0f};
  dir = {0.0f, 0.0f};

  // NOTE TO SELF: Multiple enemy instances can't share a texture as unloading
  // it causes issues
  texture = LoadTexture("../sprites/abominable_mass.png");

  speed = 80.0f;
  vel = {0.0f, 0.0f};
  knockbackVelocity = {0.0f, 0.0f};
  knockbackResistance = 0.2f;

  expDrop = 10.0f;

  width = 28.0f;
  height = 13.0f;
  hitCircle = {pos.x + width / 2.0f, pos.y + height / 2.0f, 8.0f};

  damage = 1.0f;
  health = 10.0f;
  iframes = 1.0f;
  iframeTimer = iframes;
  iframesReady = true;

  gridCells.clear();
}

void Enemy::Avoid(const std::vector<Enemy *> &nearbyEnemies) {
  for (Enemy *other : nearbyEnemies) {
    constexpr float minDistance = 16.0f;

    if (other == this) {
      continue;
    }

    const Vector2 enemyCenter = {pos.x + width / 2, pos.y + height / 2};
    const Vector2 otherCenter = {other->pos.x + other->width / 2, other->pos.y + other->height / 2};

    const Vector2 diff = Vector2Subtract(enemyCenter, otherCenter);

    if (const float distance = Vector2Length(diff); distance < minDistance && distance > 0) {
      const float overlap = minDistance - distance;
      const Vector2 direction = Vector2Normalize(diff);

      const Vector2 separation = Vector2Scale(direction, overlap * 0.5f);
      pos = Vector2Add(pos, separation);
      other->pos = Vector2Subtract(other->pos, separation);
    }
  }
}

void Enemy::Move(const Player &player) {
  const float delta = GetFrameTime();

  const Vector2 playerCenter = {player.GetPos().x + player.width / 2.0f, player.GetPos().y + player.height / 2.0f};

  const Vector2 enemyCenter = {pos.x + width / 2.0f, pos.y + height / 2.0f};

  dir = Vector2Normalize(Vector2Subtract(playerCenter, enemyCenter));

  const Vector2 movement = {dir.x * speed * delta, dir.y * speed * delta};

  pos.x += movement.x;
  pos.y += movement.y;

  hitCircle.pos.x = pos.x + width / 2.0f;
  hitCircle.pos.y = pos.y + height / 2.0f;
}

void Enemy::Receive(const Vector2 source, const float knockback, const float damage, Player &player) {
  const Vector2 enemyCenter = {hitCircle.pos.x, hitCircle.pos.y};
  Vector2 direction = Vector2Subtract(enemyCenter, source);

  if (Vector2Length(direction) > 0.0f) {
    direction = Vector2Normalize(direction);
  }

  if (iframesReady) {
    knockbackVelocity = Vector2Scale(direction, knockback * (1.0f - knockbackResistance));
    health -= damage;
    iframesReady = false;
    iframeTimer = iframes;
    Die(player);
  }
}

void Enemy::Die(Player &player) {
  for (auto it = enemies.begin(); it != enemies.end();) {
    if (it->health <= 0.0f) {
      player.expTotal += it->expDrop;
      UnloadTexture(it->texture);
      it = enemies.erase(it);
    } else {
      ++it;
    }
  }
}

void Enemy::Update() {
  const float delta = GetFrameTime();

  if (!iframesReady) {
    iframeTimer -= delta;
    if (iframeTimer <= 0.0f) {
      iframesReady = true;
    }
  }

  if (Vector2Length(knockbackVelocity) > 0.1f) {
    pos = Vector2Add(pos, Vector2Scale(knockbackVelocity, delta));
    knockbackVelocity = Vector2Lerp(knockbackVelocity, Vector2Zero(), 5.0f * delta);
  } else {
    knockbackVelocity = Vector2Zero();
  }

  const std::vector<Enemy *> nearbyEnemies = grid.GetNeighboursInRadius(this, 50.0f);
  Avoid(nearbyEnemies);

  hitCircle.pos.x = pos.x + width / 2.0f;
  hitCircle.pos.y = pos.y + height / 2.0f;
}

void Enemy::Draw() const {
  // DrawCircleLines(hitCircle.pos.x, hitCircle.pos.y, hitCircle.radius, RED);

  const Rectangle srcRect = {0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
  DrawTextureRec(texture, srcRect, pos, RAYWHITE);
}
