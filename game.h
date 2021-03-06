#ifndef GAME_H
#define GAME_H

#include "engine/geometry.h"
#include "engine/rendering.h"

#include <iostream>

typedef double seconds;

struct player_t
{
  seconds time_to_respawn;
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
  seconds ttl;
};

#define NUM_ASTEROIDS 10
struct asteroid_t
{
  bool active;
  point_t location;
  polygon_t shape;
  rotations direction;
};

struct game_state_t {
  game_state_t() : initialized(false) {}
  bool initialized;
  uint seed;
  int num_calls_to_rand;
  int lives;

  player_t player;

  laser_t lasers[NUM_LASERS];
  bool can_fire;

  asteroid_t asteroids[NUM_ASTEROIDS];
  seconds time_since_last_spawn;

  private:
    game_state_t( const game_state_t & obj); // Don't pass this by reference
};

struct frame_t
{
  double dt;
  controller_t controller;
  game_state_t game_state;
};

#endif
