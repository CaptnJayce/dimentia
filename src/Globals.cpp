#include "../include/Globals.hpp"
#include "../include/Enemy.hpp"
#include <raylib.h>

Textures textures;

int screenHeight = 1080;
int screenWidth = 1920;
float cellSize = 25.0f;
float zoomLevel = 4.0f;    // default zoom level
float volumeLevel = 25.0f; // default volume level
bool shouldClose = false;

std::unordered_set<int> gridCells;

// TODO: As game expands, I should split this up into multiple inits depending
// on the level loaded
void InitTextures() {
  // weapon textures
  textures.crescentSlashTexture = LoadTexture("../sprites/CrescentSlash.png");

  // player textures
  textures.crescentWitchIdleTexture = LoadTexture("../sprites/s_CrescentWitchIdle.png");
  textures.crescentWitchRunTexture = LoadTexture("../sprites/s_CrescentWitchRun.png");
}

void UnloadEnemyTextures(std::vector<Enemy> &enemies) {
  for (Enemy &enemy : enemies) {
    UnloadTexture(enemy.GetTexture());
  }
}
