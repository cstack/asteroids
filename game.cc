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

uint get_octant(double x1, double x2, double y1, double y2) {
   /*
   Octants:
   \2|1/
   3\|/0
  ---+---
   4/|\7
   /5|6\
   */
  double dx = x2 - x1;
  double dy = y2 - y1;
  double steepness = abs(dy/dx);
  std::cout << "dx " << dx << ", dy " << dy << ", steepness " << steepness << std::endl;
  if (dx > 0) {
    if (dy > 0) {
      if (steepness > 1) {
        return 1;
      } else {
        return 0;
      }
    } else {
      if (steepness > 1) {
        return 6;
      } else {
        return 7;
      }
    }
  } else {
    if (dy > 0) {
      if (steepness > 1) {
        return 2;
      } else {
        return 3;
      }
    } else {
      if (steepness > 1) {
        return 5;
      } else {
        return 4;
      }
    }
  }
}

screen_location_t switch_to_octant_zero_from(uint octant, double x, double y) {
  screen_location_t result;
  switch(octant) {
  case 0:
    result.x = x;
    result.y = y;
    break;
  case 1:
    result.x = y;
    result.y = x;
    break;
  case 2:
    result.x = y;
    result.y = -x;
    break;
  case 3:
    result.x = -x;
    result.y = y;
    break;
  case 4:
    result.x = -x;
    result.y = -y;
    break;
  case 5:
    result.x = -y;
    result.y = -x;
    break;
  case 6:
    result.x = -y;
    result.y = x;
    break;
  case 7:
    result.x = x;
    result.y = -y;
    break;
  default:
    assert(false, "Unhandled octant");
  }
  return result;
}

screen_location_t switch_from_octant_zero_to(uint octant, double x, double y) {
  screen_location_t result;
  switch(octant) {
  case 0:
    result.x = x;
    result.y = y;
    break;
  case 1:
    result.x = y;
    result.y = x;
    break;
  case 2:
    result.x = -y;
    result.y = x;
    break;
  case 3:
    result.x = -x;
    result.y = y;
    break;
  case 4:
    result.x = -x;
    result.y = -y;
    break;
  case 5:
    result.x = -y;
    result.y = -x;
    break;
  case 6:
    result.x = y;
    result.y = -x;
    break;
  case 7:
    result.x = x;
    result.y = -y;
    break;
  default:
    assert(false, "Unhandled octant");
  }
  return result;
}

void draw_line(screen_location_t p1, screen_location_t p2, color_t color, pixel_buffer_t* pixel_buffer) {
  // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

  std::cout << "draw_line(p1 " << p1 << ", p2 " << p2 << ", color_t color, pixel_buffer_t* pixel_buffer)" << std::endl;

  uint octant = get_octant(p1.x, p2.x, p1.y, p2.y);
  std::cout << "octant " << octant << std::endl;

  p1 = switch_to_octant_zero_from(octant, p1.x, p1.y);
  p2 = switch_to_octant_zero_from(octant, p2.x, p2.y);

  std::cout << "p1: " << p1 << std::endl;
  std::cout << "p2: " << p2 << std::endl;

  // Only works for lines in the first octant
  double dx = p2.x - p1.x;
  double dy = p2.y - p1.y;
  double error = 0;
  double deltaerr = abs(dy / dx);
  int y = p1.y;
  screen_location_t pixel_to_print;
  for (int x = p1.x; x < p2.x; x++) {
    pixel_to_print = switch_from_octant_zero_to(octant, x, y);
    put_pixel(pixel_buffer, pixel_to_print.x, pixel_to_print.y, color);
    error += deltaerr;
    while (error >= 0.5) {
      pixel_to_print = switch_from_octant_zero_to(octant, x, y);
      put_pixel(pixel_buffer, pixel_to_print.x, pixel_to_print.y, color);
      y += sign(p2.y - p1.y);
      error -= 1.0;
    }
  }

  // assert(false, "Stopping after drawing one line");
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
  screen_location_t p2 = get_screen_location(translate_without_wrapping(game_state.player_location, 3, 2));
  draw_line(
    p1,
    p2,
    RED, // Octant 0
    pixel_buffer
  );

  p2 = get_screen_location(translate_without_wrapping(game_state.player_location, 2, 3));
  draw_line(
    p1,
    p2,
    GREEN, // Octant 1
    pixel_buffer
  );

  // p2 = get_screen_location(translate_without_wrapping(game_state.player_location, -2, 3));
  // draw_line(
  //   p1,
  //   p2,
  //   BLUE, // Octant 3
  //   pixel_buffer
  // );

  // p2 = get_screen_location(translate_without_wrapping(game_state.player_location, -3, 2));
  // draw_line(
  //   p1,
  //   p2,
  //   YELLOW, // Octant 4
  //   pixel_buffer
  // );
}
