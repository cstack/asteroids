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
  explicit point_t(meters x_ = 0, meters y_ = 0):
    x(x_), y(y_) {}
  void operator+=(const point_t & other);
  meters x;
  meters y;
};

struct line_t {
  line_t(meters x1_, meters y1_, meters x2_, meters y2_):
    p1(point_t(x1_, y1_)), p2(point_t(x2_, y2_)) {}
  line_t(point_t p1_, point_t p2_):
    p1(p1_), p2(p2_) {}
  point_t p1;
  point_t p2;
};

const ushort MAX_POINTS_IN_POLYGON = 5;
struct polygon_t
{
  ushort num_points;
  point_t points[MAX_POINTS_IN_POLYGON];
};

point_t clip(point_t point, meters max_magnitude);
bool lines_intersect(const line_t & l1, const line_t & l2);
bool point_in_polygon(const point_t & point, const polygon_t & polygon);
bool line_intersects_polygon(const line_t & line, const polygon_t & polygon);
bool polygons_intersect(const polygon_t & p1, const polygon_t & p2);
polygon_t rotate(polygon_t polygon, rotations angle);

// < 0  : left of line
// == 0 : on line
// > 0  : right of line
int side_of_line(const point_t & p_test, const line_t & line);

point_t translate(point_t location, meters dx, meters dy);
point_t translate(point_t location, point_t delta);
polygon_t translate(const polygon_t & polygon, const point_t & delta);
point_t translate_and_wrap(point_t location, meters dx, meters dy);
point_t translate_and_wrap(point_t location, point_t delta);
point_t vector(meters magnitude, rotations direction);

std::ostream &operator<<(std::ostream &os, point_t const &location);
std::ostream &operator<<(std::ostream &os, polygon_t const &polygon);
std::ostream &operator<<(std::ostream &os, line_t const &line);
point_t operator*(const point_t & point, const double & scalar);
point_t operator-(const point_t & p1, const point_t & p2);
#endif
