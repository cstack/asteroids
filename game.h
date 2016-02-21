#ifndef GAME_H
#define GAME_H

#include "engine/geometry.h"
#include "engine/rendering.h"

#include <iostream>

struct player_t
{
  point_t location;
  polygon_t shape;
  rotations direction;
  point_t velocity;
};

#define NUM_LASERS 5
struct laser_t
{
  bool active;
  point_t location;
  rotations direction;
};

struct game_state_t {
  game_state_t() : initialized(false) {}
  bool initialized;

  player_t player;
  laser_t lasers[NUM_LASERS];
  bool can_fire;
  ushort laser_index;
};

#endif
