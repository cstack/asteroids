#include "game.h"

#include "platform/platform.h"
#include "engine/rendering.h"
#include "engine/util.h"

game_state_t game_state;

const meters PLAYER_MAX_SPEED_METERS_PER_SECOND = 8;
const rotations PLAYER_ROTATIONS_PER_SECOND = 1;
const double PLAYER_ACCELERATION = 1;
const color_t PLAYER_COLOR = rgb(200, 200, 255);

void initialize_game_state(game_state_t &game_state) {
  game_state.player.location.x = SCREEN_WIDTH/2;
  game_state.player.location.y = SCREEN_HEIGHT/2;

  // Triangle
  game_state.player.shape.num_points = 3;
  game_state.player.shape.points[0].x = 1;
  game_state.player.shape.points[0].y = 0;
  game_state.player.shape.points[1].x = -1;
  game_state.player.shape.points[1].y = -1;
  game_state.player.shape.points[2].x = -1;
  game_state.player.shape.points[2].y = 1;

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

  // Accelerate player
  game_state.player.direction = wrap(game_state.player.direction + delta_direction, 0, 1);
  if (controller.up_pressed) {
    game_state.player.velocity = translate(
      game_state.player.velocity,
      vector(
        PLAYER_ACCELERATION * dt,
        game_state.player.direction
      )
    );
  }

  // Move player
  game_state.player.location = translate(
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
