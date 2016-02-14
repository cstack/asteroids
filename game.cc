#include "game.h"

#include "platform/platform.h"
#include "engine/util.h"

game_state_t game_state;

const double METERS_TO_PIXELS = 32; // Multiply by this to convert meters to pixels
const double PIXELS_TO_METERS = 1/METERS_TO_PIXELS;

const meters PLAYER_SPEED_METERS_PER_SECOND = 8;
const color_t PLAYER_COLOR = rgb(200, 200, 255);

void put_pixel(pixel_buffer_t* pixel_buffer, uint x, uint y, color_t color) {
  uint offset = pixel_buffer->width * y + x;
  pixel_buffer->data[offset] = color;
}

void draw_box(pixel_buffer_t* pixel_buffer, double x, double y, double width, double height, color_t color) {
  int x1 = clip(x, 0, pixel_buffer->width);
  int x2 = clip(x+width, 0, pixel_buffer->width);
  int y1 = clip(y, 0, pixel_buffer->height);
  int y2 = clip(y+height, 0, pixel_buffer->height);

  for (int x_to_fill=x1; x_to_fill<x2; x_to_fill++) {
    for (int y_to_fill=y1; y_to_fill<y2; y_to_fill++) {
      put_pixel(pixel_buffer, x_to_fill, y_to_fill, color);
    }
  }
}

void initialize_game_state(game_state_t &game_state) {
  game_state.player_location.x = 0;
  game_state.player_location.y = 0;

  game_state.initialized = true;
}

void clear_screen(pixel_buffer_t* pixel_buffer) {
  draw_box(pixel_buffer, 0, 0, pixel_buffer->width, pixel_buffer->height, BLACK);
}

screen_location_t get_screen_location(location_t location) {
  screen_location_t screen_location;
  screen_location.x = location.x * METERS_TO_PIXELS;
  screen_location.y = location.y * METERS_TO_PIXELS;
  return screen_location;
}

void update(double dt, pixel_buffer_t* pixel_buffer, controller_t &controller) {
  if (!game_state.initialized) {
    initialize_game_state(game_state);
  }

  location_t player_location = game_state.player_location;

  clear_screen(pixel_buffer);

  // // Move player
  meters dx = 0, dy = 0;
  if (controller.right_pressed) {
    dx += PLAYER_SPEED_METERS_PER_SECOND * dt;
  }
  if (controller.left_pressed) {
    dx -= PLAYER_SPEED_METERS_PER_SECOND * dt;
  }
  if (controller.down_pressed) {
    dy += PLAYER_SPEED_METERS_PER_SECOND * dt;
  }
  if (controller.up_pressed) {
    dy -= PLAYER_SPEED_METERS_PER_SECOND * dt;
  }
  game_state.player_location.x += dx;
  game_state.player_location.y += dy;

  // Render Player
  screen_location_t screen_location = get_screen_location(player_location);
  draw_box(
    pixel_buffer,
    screen_location.x,
    screen_location.y,
    100,
    100,
    PLAYER_COLOR
  );
}
