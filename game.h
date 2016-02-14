#ifndef GAME_H
#define GAME_H

#include "engine/color.h"

#include <iostream>

const uint SCREEN_WIDTH_PIXELS = 1024;
const uint SCREEN_HEIGHT_PIXELS = 576;

typedef double meters;
typedef uint pixels;

struct location_t {
  meters x;
  meters y;
};

struct screen_location_t {
  pixels x;
  pixels y;
};

struct game_state_t {
  game_state_t() : initialized(false) {}
  bool initialized;

  location_t player_location;
};

#endif
