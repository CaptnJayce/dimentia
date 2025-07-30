#pragma once

#include <raylib.h>

// TODO: remove this struct
struct Textures {
    // weapon textures
    Texture huskOneWeaponTexture;
};

extern int screenWidth;
extern int screenHeight;
extern float cellSize;

extern Textures textures;

void InitTextures();
