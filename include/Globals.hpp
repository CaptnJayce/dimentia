#pragma once
#include <raylib.h>

struct Circle {
    Vector2 pos;
    float radius;
};

struct Textures {
    // weapon textures
    Texture huskOneWeaponTexture;

    // enemy textures
    Texture abominableMassTexture;
};

extern Textures textures;

void InitTextures();
