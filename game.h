#ifndef GAME_H
#define GAME_H

#include "engine/geometry.h"
#include "engine/rendering.h"

#include <iostream>

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
