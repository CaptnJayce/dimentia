#include "../include/AnimationManager.hpp"
#include <cmath>
#include <raylib.h>

AnimationManager::AnimationManager() : currentState(AnimState::IDLE) {}

void AnimationManager::AddAnimation(const AnimState state, const Texture &name, const int frames, const float delay, const bool loops) {
  Animation anim{};
  anim.sheet = Texture(name);
  anim.frameCount = frames;
  anim.frameDelay = delay;
  anim.loop = loops;
  anim.currentFrame = 0;
  anim.frameCounter = 0;
  anim.frameRec = {0, 0, static_cast<float>(anim.sheet.width) / frames, static_cast<float>(anim.sheet.height)};
  animations[state] = anim;
}

void AnimationManager::Play(const AnimState state, const bool forceRestart) {
  if (currentState != state || forceRestart) {
    currentState = state;
    auto &anim = animations[state];
    anim.currentFrame = 0;
    anim.frameCounter = 0;
  }
}

void AnimationManager::Update() {
  auto &anim = animations[currentState];
  anim.frameCounter += GetFrameTime();

  if (anim.frameCounter >= anim.frameDelay) {
    anim.frameCounter = 0;
    anim.currentFrame++;

    if (anim.currentFrame >= anim.frameCount) {
      if (anim.loop) {
        anim.currentFrame = 0;
      }
    }

    anim.frameRec.x = anim.currentFrame * anim.frameRec.width;
  }
}

void AnimationManager::Draw(const Vector2 position, const Camera2D &camera, const bool flipX) {
  const auto &anim = animations[currentState];
  Rectangle source = anim.frameRec;

  if (flipX) {
    source.width *= -1;
  }

  Vector2 screenPos = GetWorldToScreen2D(position, camera);

  screenPos.x = floorf(screenPos.x);
  screenPos.y = floorf(screenPos.y);

  Vector2 snappedWorld = GetScreenToWorld2D(screenPos, camera);

  DrawTextureRec(anim.sheet, source, snappedWorld, WHITE);
}
