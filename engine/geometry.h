#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

typedef double meters;
const meters SCREEN_WIDTH = 32;
const meters SCREEN_HEIGHT = 18;

typedef double rotations;
const rotations RIGHT = 0;
const rotations UP = 0.25;
const rotations LEFT = 0.5;
const rotations DOWN = 0.75;

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

point_t translate_and_wrap(point_t location, meters dx, meters dy);
point_t translate_and_wrap(point_t location, point_t delta);
point_t translate(point_t location, meters dx, meters dy);
point_t translate(point_t location, point_t delta);
polygon_t rotate(polygon_t polygon, rotations angle);
std::ostream &operator<<(std::ostream &os, point_t const &location);
std::ostream &operator<<(std::ostream &os, polygon_t const &polygon);
point_t vector(meters magnitude, rotations direction);
point_t clip(point_t point, meters max_magnitude);

#endif
