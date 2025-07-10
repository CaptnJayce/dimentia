#pragma once
#include <raylib.h>
#include <unordered_map>

enum class AnimState {
    IDLE,
    RUN,
};

struct Animation {
    Texture2D sheet;
    Rectangle frameRec;
    int frameCount;
    int currentFrame;
    float frameCounter;
    float frameDelay;
    bool loop;
};

class AnimationManager {
  public:
    AnimationManager();
    //~AnimationManager();

    void AddAnimation(AnimState, const char *path, int frames, float delay, bool loops);
    void Play(AnimState state, bool forceRestart = false);
    void Update();
    void Draw(Vector2 position, bool flipX = false);
    Rectangle GetCurrentFrame() const;

    AnimState GetCurrentState() const { return currentState; }

  private:
    std::unordered_map<AnimState, Animation> animations;
    AnimState currentState;
};
