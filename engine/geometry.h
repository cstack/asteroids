#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

typedef double meters;

const meters SCREEN_WIDTH = 32;
const meters SCREEN_HEIGHT = 18;

struct point_t {
  // Location in space relative to origin (bottom left of screen)
  meters x;
  meters y;
};

const ushort MAX_POINTS_IN_POLYGON = 5;
struct polygon_t
{
  ushort num_points;
  point_t points[MAX_POINTS_IN_POLYGON];
};

point_t translate(point_t location, meters dx, meters dy);
point_t translate(point_t location, point_t delta);
point_t translate_without_wrapping(point_t location, meters dx, meters dy);
point_t translate_without_wrapping(point_t location, point_t delta);
std::ostream &operator<<(std::ostream &os, point_t const &location);
std::ostream &operator<<(std::ostream &os, polygon_t const &polygon);

#endif
