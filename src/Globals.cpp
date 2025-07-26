#include "../include/Globals.hpp"
#include <raylib.h>

Textures textures;

// TODO: As game expands, I should split this up into multiple inits depending on the level loaded
void InitTextures() {
    // weapon textures
    textures.huskOneWeaponTexture = LoadTexture("../sprites/s_CrescentSlashOne.png");

    // enemy textures
    textures.abominableMassTexture = LoadTexture("../sprites/abominable_mass.png");
}
