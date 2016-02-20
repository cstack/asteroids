#ifndef GAME_H
#define GAME_H

#include "engine/color.h"

#include <iostream>

const uint SCREEN_WIDTH_PIXELS = 1024;
const uint SCREEN_HEIGHT_PIXELS = 576;

typedef double meters;
typedef int pixels;

struct point_t {
  // Location in space relative to origin (bottom left of screen)
  meters x;
  meters y;
};

struct screen_point_t {
  // Index of pixel relative to top left of screen
  pixels x;
  pixels y;
};

const ushort MAX_POINTS_IN_POLYGON = 5;
struct polygon_t
{
  ushort num_points;
  point_t points[MAX_POINTS_IN_POLYGON];
};

struct player_t
{
  point_t location;
  polygon_t shape;
};

struct game_state_t {
  game_state_t() : initialized(false) {}
  bool initialized;

  player_t player;
};

#endif
