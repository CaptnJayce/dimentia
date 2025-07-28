#pragma once

#include <raylib.h>

struct Textures {
    // weapon textures
    Texture huskOneWeaponTexture;

    // enemy textures
    Texture abominableMassTexture;
};

extern int screenWidth;
extern int screenHeight;
extern float cellSize;

extern Textures textures;

void InitTextures();
