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

#define NUM_ASTEROIDS 10
struct asteroid_t
{
  bool active;
  point_t location;
  polygon_t shape;
  rotations direction;
};

typedef double seconds;

struct game_state_t {
  game_state_t() : initialized(false) {}
  bool initialized;
  uint seed;
  int num_calls_to_rand;

  player_t player;

  laser_t lasers[NUM_LASERS];
  bool can_fire;
  ushort laser_index;

  asteroid_t asteroids[NUM_ASTEROIDS];
  ushort asteroid_index;
  seconds time_since_last_spawn;
};

struct frame_t
{
  double dt;
  controller_t controller;
  game_state_t game_state;
};

#endif
