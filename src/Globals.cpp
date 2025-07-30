#include "../include/Globals.hpp"
#include <raylib.h>

Textures textures;

int screenHeight = 1080;
int screenWidth = 1920;
float cellSize = 25.0f;

// TODO: As game expands, I should split this up into multiple inits depending on the level loaded
void InitTextures() {
    // weapon textures
    textures.huskOneWeaponTexture = LoadTexture("../sprites/crescent_slash.png");
}
