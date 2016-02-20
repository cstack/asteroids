#include "rendering.h"
#include "util.h"
#include "math.h"

const double METERS_TO_PIXELS = SCREEN_WIDTH_PIXELS / SCREEN_WIDTH; // Multiply by this to convert meters to pixels
const double PIXELS_TO_METERS = 1/METERS_TO_PIXELS;

struct screen_point_t {
  // Index of pixel relative to top left of screen
  pixels x;
  pixels y;
};

std::ostream &operator<<(std::ostream &os, screen_point_t const &screen_location) {
  return os << "(" << screen_location.x << ", " << screen_location.y << ")";
}

void clear_screen(pixel_buffer_t* pixel_buffer) {
  draw_box(pixel_buffer, 0, 0, pixel_buffer->width, pixel_buffer->height, BLACK);
}

color_t rgb(double red, double green, double blue) {
  color_t red_component = ((int)round(red)) << 16;
  color_t green_component = ((int)round(green)) << 8;
  color_t blue_component = ((int)round(blue));
  return red_component | green_component | blue_component;
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