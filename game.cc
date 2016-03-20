#include "game.h"

#include "platform/platform.h"
#include "engine/rendering.h"
#include "engine/util.h"
#include "math.h"

#ifdef DEBUG
  #include "engine/encoding.h"
#endif

game_state_t game_state;

const meters PLAYER_MAX_SPEED_METERS_PER_SECOND = 0.3;
const rotations PLAYER_ROTATIONS_PER_SECOND = 0.5;
const double PLAYER_ACCELERATION = 0.4;
const double PLAYER_DECELERATION = 0.2;
const color_t PLAYER_COLOR = rgb(200, 200, 255);

const meters LASER_LENGTH = 1;
const color_t LASER_COLOR = rgb(255, 100, 100);
const meters LASER_SPEED = 15;
const seconds LASER_TTL = 1;

const seconds TIME_PER_ASTEROID_SPAWN = 2;
const meters ASTEROID_SPEED = 0.6;
const color_t ASTEROID_COLOR = rgb(200, 100, 200);

std::ostream &operator<<(std::ostream &os, asteroid_t const &asteroid) {
  return os << "asteroid " <<
    (asteroid.active ? "active" : "inactive") <<
    " " << asteroid.location <<
    " " << asteroid.shape;
}

void initialize_game_state(game_state_t &game_state) {
  uint seed = time(nullptr);
  srand(seed);
  game_state.seed = seed;

  game_state.player.location.x = SCREEN_WIDTH/2;
  game_state.player.location.y = SCREEN_HEIGHT/2;

  // Triangle
  game_state.player.shape.num_points = 3;
  game_state.player.shape.points[0].x = 0.5;
  game_state.player.shape.points[0].y = 0;
  game_state.player.shape.points[1].x = -0.5;
  game_state.player.shape.points[1].y = -0.3;
  game_state.player.shape.points[2].x = -0.5;
  game_state.player.shape.points[2].y = 0.3;

  game_state.player.direction = UP;

  game_state.initialized = true;
}

uint find_available_slot(const asteroid_t * asteroids, uint num_asteroids) {
  for (uint i=0; i < num_asteroids; i++) {
    if (!asteroids[i].active) {
      return i;
    }
  }
  return -1;
}

uint find_available_slot(const laser_t * lasers, uint num_lasers) {
  for (uint i=0; i < num_lasers; i++) {
    if (!lasers[i].active) {
      return i;
    }
  }
  return -1;
}

void debugger() {} // Use this to place a breakpoint when running `make debug`

void update(double dt, pixel_buffer_t* pixel_buffer, controller_t &controller) {
  if (!game_state.initialized) {
    initialize_game_state(game_state);
  }
  game_state.num_calls_to_rand = get_num_calls_to_rand();
  #ifdef DEBUG
    frame_t frame;
    frame.dt = dt;
    frame.controller = controller;
    frame.game_state = game_state;
    printf("Frame\n");
    print_hex_encoded((uint*) &frame, sizeof(frame));
  #endif

  clear_screen(pixel_buffer);

  // Rotate player
  double delta_direction = 0;
  if (controller.right_pressed) {
    delta_direction -= PLAYER_ROTATIONS_PER_SECOND * dt;
  }
  if (controller.left_pressed) {
    delta_direction += PLAYER_ROTATIONS_PER_SECOND * dt;
  }
  game_state.player.direction = wrap(game_state.player.direction + delta_direction, 0, 1);

  // Accelerate player
  point_t delta_v;
  if (controller.up_pressed) {
    delta_v += vector(
      PLAYER_ACCELERATION * dt,
      game_state.player.direction
    );
  }

  delta_v += game_state.player.velocity * -1 * dt;

  point_t new_v = translate(
    game_state.player.velocity,
    delta_v
  );
  point_t clipped_v = clip(
    new_v,
    PLAYER_MAX_SPEED_METERS_PER_SECOND
  );
  game_state.player.velocity = clipped_v;

  // Fire Laser
  if (controller.jump_pressed && game_state.can_fire) {
    uint i = find_available_slot(game_state.lasers, NUM_LASERS);
    if (i != -1) {
      game_state.can_fire = false;
      laser_t& laser = game_state.lasers[i];
      laser.active = true;
      laser.location = game_state.player.location;
      laser.direction = game_state.player.direction;
      laser.ttl = LASER_TTL;
    }
  } else if (!controller.jump_pressed) {
    game_state.can_fire = true;
  }

  // Spawn asteroids
  game_state.time_since_last_spawn += dt;
  if (game_state.time_since_last_spawn >= TIME_PER_ASTEROID_SPAWN) {
    game_state.time_since_last_spawn -= TIME_PER_ASTEROID_SPAWN;
    uint i = find_available_slot(game_state.asteroids, NUM_ASTEROIDS);
    if (i != -1) {
      asteroid_t& asteroid = game_state.asteroids[i];
      asteroid.active = true;
      asteroid.location = translate(game_state.player.location, SCREEN_WIDTH*rand(-0.5, 0.5), SCREEN_HEIGHT*rand(-0.5, 0.5));
      asteroid.direction = rand(0,1);
      asteroid.shape.num_points = 5;
      asteroid.shape.points[0] = vector(rand(0.2, 1), 1-0);
      asteroid.shape.points[1] = vector(rand(0.2, 1), 1-0.2);
      asteroid.shape.points[2] = vector(rand(0.2, 1), 1-0.3);
      asteroid.shape.points[3] = vector(rand(0.2, 1), 1-0.6);
      asteroid.shape.points[4] = vector(rand(0.2, 1), 1-0.9);
    }
  }

  // Move and render lasers
  for (int i = 0; i < NUM_LASERS; i++) {
    laser_t& laser = game_state.lasers[i];
    if (laser.active) {
      laser.location = translate_and_wrap(laser.location, vector(LASER_SPEED*dt, laser.direction));
      draw_line(
        pixel_buffer,
        laser.location,
        translate(laser.location, vector(LASER_LENGTH, laser.direction)),
        LASER_COLOR
      );

      laser.ttl -= dt;
      if (laser.ttl <= 0) {
        laser.active = false;
      }
    }
  }

  // Move and render asteroids
  for (int i = 0; i < NUM_ASTEROIDS; i++) {
    asteroid_t& asteroid = game_state.asteroids[i];
    if (asteroid.active) {
      asteroid.location = translate_and_wrap(asteroid.location, vector(ASTEROID_SPEED*dt, asteroid.direction));

      draw_polygon(
        pixel_buffer,
        asteroid.location,
        rotate(asteroid.shape, asteroid.direction),
        ASTEROID_COLOR
      );
    }
  }

  // Collide lasers with asteroids
  for (int i = 0; i < NUM_ASTEROIDS; i++) {
    asteroid_t& asteroid = game_state.asteroids[i];
    if (asteroid.active) {
      polygon_t collison_box = rotate(asteroid.shape, asteroid.direction);
      for (int j = 0; j < NUM_LASERS; j++) {
        laser_t& laser = game_state.lasers[j];
        if (laser.active) {
          point_t laser_back = translate(laser.location, asteroid.location.x * -1, asteroid.location.y * -1);
          point_t laser_front = translate(laser_back, vector(LASER_LENGTH, laser.direction));
          line_t laser_line = line_t(laser_back, laser_front);

          if (line_intersects_polygon(laser_line, collison_box)) {
            asteroid.active = false;
            laser.active = false;
            break;
          }
        }
      }
    }
  }

  // Move player
  game_state.player.location = translate_and_wrap(
    game_state.player.location,
    game_state.player.velocity
  );

  // Render Player
  draw_polygon(
    pixel_buffer,
    game_state.player.location,
    rotate(game_state.player.shape, game_state.player.direction),
    PLAYER_COLOR
  );

  #ifdef DEBUG
    assert(
      isfinite(game_state.player.location.x) &&
      isfinite(game_state.player.location.y),
      "Player location has nan or inf values"
    );
  #endif
}
