#ifndef RENDERING_H
#define RENDERING_H

#include <stdint.h>
#include "../platform/platform.h"
#include "geometry.h"

typedef uint32_t color_t;
typedef int pixels;

const pixels SCREEN_WIDTH_PIXELS = 1024;
const pixels SCREEN_HEIGHT_PIXELS = 576;

const color_t BLACK = 0x00000000;
const color_t BLUE = 0x000000FF;
const color_t GREEN = 0x0000FF00;
const color_t CYAN = 0x0000FFFF;
const color_t RED = 0x00FF0000;
const color_t MAGENTA = 0x00FF00FF;
const color_t YELLOW = 0x00FFFF00;
const color_t WHITE = 0x00FFFFFF;

struct screen_point_t {
  // Index of pixel relative to top left of screen
  pixels x;
  pixels y;
};

color_t rgb(double red, double green, double blue);
void draw_box(pixel_buffer_t* pixel_buffer, double x, double y, double width, double height, color_t color);
void draw_polygon(pixel_buffer_t* pixel_buffer, point_t location, polygon_t polygon, color_t color);
std::ostream &operator<<(std::ostream &os, screen_point_t const &screen_location);

#endif
