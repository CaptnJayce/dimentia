#include "../include/AnimationManager.hpp"
#include <raylib.h>

AnimationManager::AnimationManager() : currentState(AnimState::IDLE) {}

void AnimationManager::AddAnimation(AnimState state, const char *path, int frames, float delay, bool loops) {
    Animation anim;
    anim.sheet = LoadTexture(path);
    anim.frameCount = frames;
    anim.frameDelay = delay;
    anim.loop = loops;
    anim.currentFrame = 0;
    anim.frameCounter = 0;
    anim.frameRec = {0, 0, (float)anim.sheet.width / frames, (float)anim.sheet.height};
    animations[state] = anim;
}

void AnimationManager::Play(AnimState state, bool forceRestart) {
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

void AnimationManager::Draw(Vector2 position, bool flipX) {
    auto &anim = animations[currentState];
    Rectangle source = anim.frameRec;
    if (flipX) {
        source.width *= -1;
    }
    DrawTextureRec(anim.sheet, source, position, WHITE);
}
