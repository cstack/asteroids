#include "game.h"

#include "platform/platform.h"
#include "engine/rendering.h"
#include "engine/util.h"

game_state_t game_state;

const meters PLAYER_MAX_SPEED_METERS_PER_SECOND = 0.5;
const rotations PLAYER_ROTATIONS_PER_SECOND = 1;
const double PLAYER_ACCELERATION = 0.5;
const color_t PLAYER_COLOR = rgb(200, 200, 255);

const meters LASER_LENGTH = 1;
const color_t LASER_COLOR = rgb(255, 100, 100);
const meters LASER_SPEED = 1;

void initialize_game_state(game_state_t &game_state) {
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

void update(double dt, pixel_buffer_t* pixel_buffer, controller_t &controller) {
  if (!game_state.initialized) {
    initialize_game_state(game_state);
  }

  clear_screen(pixel_buffer);

  // Rotate player
  double delta_direction;
  if (controller.right_pressed) {
    delta_direction -= PLAYER_ROTATIONS_PER_SECOND * dt;
  }
  if (controller.left_pressed) {
    delta_direction += PLAYER_ROTATIONS_PER_SECOND * dt;
  }
  game_state.player.direction = wrap(game_state.player.direction + delta_direction, 0, 1);

  // Accelerate player
  if (controller.up_pressed) {
    std::cout << "up pressed" << std::endl;
    point_t delta_v = vector(
      PLAYER_ACCELERATION * dt,
      game_state.player.direction
    );
    point_t new_v = translate(
      game_state.player.velocity,
      delta_v
    );
    point_t clipped_v = clip(
      new_v,
      PLAYER_MAX_SPEED_METERS_PER_SECOND
    );
    game_state.player.velocity = clipped_v;
  }

  // Fire Laser
  if (controller.jump_pressed && game_state.can_fire) {
    std::cout << "Fire!" << std::endl;
    game_state.can_fire = false;
    laser_t& laser = game_state.lasers[game_state.laser_index];
    laser.active = true;
    laser.location = game_state.player.location;
    laser.direction = game_state.player.direction;
    game_state.laser_index = (game_state.laser_index + 1) % NUM_LASERS;
  } else if (!controller.jump_pressed) {
    game_state.can_fire = true;
  }

  // Move and render lasers
  for (int i = 0; i < NUM_LASERS; i++) {
    laser_t& laser = game_state.lasers[i];
    if (laser.active) {
      laser.location = translate(laser.location, vector(LASER_SPEED, laser.direction));
      draw_line(
        pixel_buffer,
        laser.location,
        translate(laser.location, vector(LASER_LENGTH, laser.direction)),
        LASER_COLOR
      );
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
}
