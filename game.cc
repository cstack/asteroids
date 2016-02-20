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
std::ostream &operator<<(std::ostream &os, screen_point_t const &screen_location) {
  return os << "(" << screen_location.x << ", " << screen_location.y << ")";
}
std::ostream &operator<<(std::ostream &os, point_t const &location) {
  return os << "(" << location.x << ", " << location.y << ")";
}
std::ostream &operator<<(std::ostream &os, polygon_t const &polygon) {
  os << "polygon(" << polygon.num_points << ")";
  for (int i = 0; i < polygon.num_points; i++) {
    os << " " << polygon.points[i];
  }
  return os;
}

void put_pixel(pixel_buffer_t* pixel_buffer, uint x, uint y, color_t color) {
  x %= pixel_buffer->width;
  y %= pixel_buffer->height;
  y = SCREEN_HEIGHT_PIXELS - y;
  uint offset = pixel_buffer->width * y + x;
  pixel_buffer->data[offset] = color;
}

point_t translate(point_t location, meters dx, meters dy) {
  location.x = wrap(location.x + dx, 0, SCREEN_WIDTH);
  location.y = wrap(location.y + dy, 0, SCREEN_HEIGHT);
  return location;
}

point_t translate(point_t location, point_t delta) {
  location.x = wrap(location.x + delta.x, 0, SCREEN_WIDTH);
  location.y = wrap(location.y + delta.y, 0, SCREEN_HEIGHT);
  return location;
}

point_t translate_without_wrapping(point_t location, meters dx, meters dy) {
  location.x = location.x + dx;
  location.y = location.y + dy;
  return location;
}

point_t translate_without_wrapping(point_t location, point_t delta) {
  location.x = location.x + delta.x;
  location.y = location.y + delta.y;
  return location;
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

screen_point_t switch_to_octant_zero_from(uint octant, double x, double y) {
  screen_point_t result;
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

screen_point_t switch_from_octant_zero_to(uint octant, double x, double y) {
  screen_point_t result;
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

void draw_line(screen_point_t p1, screen_point_t p2, color_t color, pixel_buffer_t* pixel_buffer) {
  // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

  uint octant = get_octant(p1.x, p2.x, p1.y, p2.y);

  p1 = switch_to_octant_zero_from(octant, p1.x, p1.y);
  p2 = switch_to_octant_zero_from(octant, p2.x, p2.y);

  // Only works for lines in the first octant
  double dx = p2.x - p1.x;
  double dy = p2.y - p1.y;
  double error = 0;
  double deltaerr = abs(dy / dx);
  int y = p1.y;
  screen_point_t pixel_to_print;
  for (int x = p1.x; x < p2.x; x++) {
    pixel_to_print = switch_from_octant_zero_to(octant, x, y);
    put_pixel(pixel_buffer, wrap(pixel_to_print.x, 0, SCREEN_WIDTH_PIXELS), wrap(pixel_to_print.y, 0, SCREEN_HEIGHT_PIXELS), color);
    error += deltaerr;
    while (error >= 0.5) {
      pixel_to_print = switch_from_octant_zero_to(octant, x, y);
      put_pixel(pixel_buffer, wrap(pixel_to_print.x, 0, SCREEN_WIDTH_PIXELS), wrap(pixel_to_print.y, 0, SCREEN_HEIGHT_PIXELS), color);
      y += sign(p2.y - p1.y);
      error -= 1.0;
    }
  }
}

screen_point_t get_screen_location(point_t location) {
  screen_point_t screen_location;
  screen_location.x = location.x * METERS_TO_PIXELS;
  screen_location.y = location.y * METERS_TO_PIXELS;
  return screen_location;
}

void draw_polygon(pixel_buffer_t* pixel_buffer, point_t location, polygon_t polygon, color_t color) {
  for (int i = 1; i < polygon.num_points; i++) {
    screen_point_t p1 = get_screen_location(translate_without_wrapping(location, polygon.points[i-1]));
    screen_point_t p2 = get_screen_location(translate_without_wrapping(location, polygon.points[i]));
    draw_line(p1, p2, color, pixel_buffer);
  }
  screen_point_t p1 = get_screen_location(translate_without_wrapping(location, polygon.points[polygon.num_points-1]));
  screen_point_t p2 = get_screen_location(translate_without_wrapping(location, polygon.points[0]));
  draw_line(p1, p2, color, pixel_buffer);
}

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
