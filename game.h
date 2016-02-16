#ifndef GAME_H
#define GAME_H

#include "engine/color.h"

#include <iostream>

const uint SCREEN_WIDTH_PIXELS = 1024;
const uint SCREEN_HEIGHT_PIXELS = 576;

typedef double meters;
typedef int pixels;

struct location_t {
  // Location in space relative to origin (bottom left of screen)
  meters x;
  meters y;
};

struct screen_location_t {
  // Index of pixel relative to top left of screen
  pixels x;
  pixels y;
};

struct game_state_t {
  game_state_t() : initialized(false) {}
  bool initialized;

  location_t player_location;
};

#endif
