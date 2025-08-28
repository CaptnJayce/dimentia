#pragma once

#include "Entity.hpp"
#include "Player.hpp"
#include <raylib.h>
#include <unordered_set>
#include <vector>

// NOTE TO SELF: this is necessary for games future
// as more weapons/character sprites get added, we'll use this struct to pick
// what loads depending on character & weapon selection
struct Textures {
  // weapon textures
  Texture crescentSlashTexture;

  // character textures
  Texture crescentWitchIdleTexture;
  Texture crescentWitchRunTexture;
};

extern std::unordered_set<int> gridCells;

extern int screenWidth;
extern int screenHeight;
extern float cellSize;
extern float zoomLevel;
extern float volumeLevel;
extern bool shouldClose;

extern Textures textures;

void InitTextures();
void InitSettings(Player &player);
void UnloadEnemyTextures(std::vector<Enemy> &enemies);
