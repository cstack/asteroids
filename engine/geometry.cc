#include "geometry.h"
#include "util.h"
#include <math.h>

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

double radians(rotations angle) {
  return angle * 2 * M_PI;
}

point_t rotate(point_t point, rotations angle) {
  double r = radians(angle);
  double c = cos(r);
  double s = sin(r);
  meters xnew = point.x * c - point.y * s;
  meters ynew = point.x * s + point.y * c;
  point.x = xnew;
  point.y = ynew;
  return point;
}

polygon_t rotate(polygon_t polygon, rotations angle) {
  for (int i = 0; i < polygon.num_points; i++) {
    polygon.points[i] = rotate(polygon.points[i], angle);
  }
  return polygon;
}

point_t vector(meters magnitude, rotations direction) {
  point_t result;
  result.x = magnitude * cos(radians(direction));
  result.y = magnitude * sin(radians(direction));
  return result;
}