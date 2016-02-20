#include "game.h"

#include "platform/platform.h"
#include "engine/rendering.h"
#include "engine/util.h"

game_state_t game_state;

const meters PLAYER_SPEED_METERS_PER_SECOND = 8;
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

  game_state.initialized = true;
}

void clear_screen(pixel_buffer_t* pixel_buffer) {
  draw_box(pixel_buffer, 0, 0, pixel_buffer->width, pixel_buffer->height, BLACK);
}

void update(double dt, pixel_buffer_t* pixel_buffer, controller_t &controller) {
  if (!game_state.initialized) {
    initialize_game_state(game_state);
  }

  clear_screen(pixel_buffer);

  // Move player
  meters dx = 0, dy = 0;
  if (controller.right_pressed) {
    dx += PLAYER_SPEED_METERS_PER_SECOND * dt;
  }
  if (controller.left_pressed) {
    dx -= PLAYER_SPEED_METERS_PER_SECOND * dt;
  }
  if (controller.up_pressed) {
    dy += PLAYER_SPEED_METERS_PER_SECOND * dt;
  }
  if (controller.down_pressed) {
    dy -= PLAYER_SPEED_METERS_PER_SECOND * dt;
  }
  game_state.player.location = translate(game_state.player.location, dx, dy);

  // Render Player
  draw_polygon(
    pixel_buffer,
    game_state.player.location,
    game_state.player.shape,
    PLAYER_COLOR
  );
}
