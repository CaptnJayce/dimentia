#pragma once

#include <raylib.h>

// NOTE TO SELF: this is necessary for games future
// as more weapons/character sprites get added, we'll use this struct to pick what loads depending on
// character & weapon selection
struct Textures {
    // weapon textures
    Texture crescentSlashTexture;

    // character textures
    Texture crescentWitchIdleTexture;
    Texture crescentWitchRunTexture;
};

extern int screenWidth;
extern int screenHeight;
extern float cellSize;

extern Textures textures;

void InitTextures();
