#include "game.h"

#include "platform/platform.h"
#include "engine/util.h"

game_state_t game_state;

const double METERS_TO_PIXELS = 32; // Multiply by this to convert meters to pixels
const double PIXELS_TO_METERS = 1/METERS_TO_PIXELS;

const meters SCREEN_WIDTH = SCREEN_WIDTH_PIXELS * PIXELS_TO_METERS;
const meters SCREEN_HEIGHT = SCREEN_HEIGHT_PIXELS * PIXELS_TO_METERS;

const meters PLAYER_SPEED_METERS_PER_SECOND = 8;
const color_t PLAYER_COLOR = rgb(200, 200, 255);
const meters PLAYER_WIDTH = 2;
const meters PLAYER_HEIGHT = 2;
const pixels PLAYER_WIDTH_PIXELS = PLAYER_WIDTH * METERS_TO_PIXELS;
const pixels PLAYER_HEIGHT_PIXELS = PLAYER_HEIGHT * METERS_TO_PIXELS;

// Helpful for debugging
std::ostream &operator<<(std::ostream &os, screen_location_t const &screen_location) {
    return os << "(" << screen_location.x << ", " << screen_location.y << ")";
}

void put_pixel(pixel_buffer_t* pixel_buffer, uint x, uint y, color_t color) {
  x %= pixel_buffer->width;
  y %= pixel_buffer->height;
  y = SCREEN_HEIGHT_PIXELS - y;
  uint offset = pixel_buffer->width * y + x;
  pixel_buffer->data[offset] = color;
}

void draw_box(pixel_buffer_t* pixel_buffer, double x, double y, double width, double height, color_t color) {
  int x2 = x + width;
  int y2 = y + width;

  for (int x_to_fill=x; x_to_fill<x2; x_to_fill++) {
    for (int y_to_fill=y; y_to_fill<y2; y_to_fill++) {
      put_pixel(pixel_buffer, x_to_fill, y_to_fill, color);
    }
  }
}

void draw_line(screen_location_t p1, screen_location_t p2, color_t color, pixel_buffer_t* pixel_buffer) {
  // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

  // Only works for lines in the first octant
  double dx = p2.x - p1.x;
  double dy = p2.y - p1.y;
  double error = 0;
  double deltaerr = abs(dy / dx);
  int y = p1.y;
  for (int x = p1.x; x < p2.x; x++) {
    put_pixel(pixel_buffer, x, y, color);
    error += deltaerr;
    while (error >= 0.5) {
         put_pixel(pixel_buffer, x, y, color);
         y += sign(p2.y - p1.y);
         error -= 1.0;
    }
  }
}

void initialize_game_state(game_state_t &game_state) {
  game_state.player_location.x = SCREEN_WIDTH/2;
  game_state.player_location.y = SCREEN_HEIGHT/2;

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

location_t translate(location_t location, meters dx, meters dy) {
  location.x = wrap(location.x + dx, 0, SCREEN_WIDTH);
  location.y = wrap(location.y + dy, 0, SCREEN_HEIGHT);
  return location;
}

location_t translate_without_wrapping(location_t location, meters dx, meters dy) {
  location.x = location.x + dx;
  location.y = location.y + dy;
  return location;
}

void update(double dt, pixel_buffer_t* pixel_buffer, controller_t &controller) {
  if (!game_state.initialized) {
    initialize_game_state(game_state);
  }

  location_t player_location = game_state.player_location;

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
  game_state.player_location = translate(game_state.player_location, dx, dy);

  // Render Player
  location_t player_bottom_left = translate(game_state.player_location, -PLAYER_WIDTH/2, -PLAYER_HEIGHT/2);
  screen_location_t screen_location = get_screen_location(player_bottom_left);
  draw_box(
    pixel_buffer,
    screen_location.x,
    screen_location.y,
    PLAYER_WIDTH_PIXELS,
    PLAYER_HEIGHT_PIXELS,
    PLAYER_COLOR
  );

  screen_location_t p1 = get_screen_location(game_state.player_location);
  screen_location_t p2 = get_screen_location(translate_without_wrapping(game_state.player_location, 2, 3));
  draw_line(
    p1,
    p2,
    PLAYER_COLOR,
    pixel_buffer
  );
}
