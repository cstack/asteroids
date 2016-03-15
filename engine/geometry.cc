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

std::ostream &operator<<(std::ostream &os, line_t const &line) {
  return os << "line(" << line.p1 << ", " << line.p2 << ")";
}

point_t translate_and_wrap(point_t location, meters dx, meters dy) {
  location.x = wrap(location.x + dx, 0, SCREEN_WIDTH);
  location.y = wrap(location.y + dy, 0, SCREEN_HEIGHT);
  return location;
}

point_t translate_and_wrap(point_t location, point_t delta) {
  location.x = wrap(location.x + delta.x, 0, SCREEN_WIDTH);
  location.y = wrap(location.y + delta.y, 0, SCREEN_HEIGHT);
  return location;
}

point_t translate(point_t location, meters dx, meters dy) {
  location.x = location.x + dx;
  location.y = location.y + dy;
  return location;
}

point_t translate(point_t location, point_t delta) {
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

double magnitude(point_t point) {
  return sqrt(pow(point.x, 2) + pow(point.y, 2));
}

point_t clip(point_t point, meters max_magnitude) {
  double m = magnitude(point);
  if (m > max_magnitude) {
    double scale = max_magnitude / m;
    point.x *= scale;
    point.y *= scale;
  }
  return point;
}

int side_of_line(point_t p, line_t line) {
  if (line.p1.x == line.p2.x) {
    // Vertical line
    if (line.p2.y > line.p1.y) {
      // Going up
      return sign(p.x - line.p1.x);
    } else {
      // Going down
      return sign(line.p1.x - p.x);
    }
  }
  double m = (line.p2.y - line.p1.y) / (line.p2.x - line.p1.x);
  double b = line.p1.y - m * line.p1.x;
  int point_above_line = sign(p.y - (m*p.x + b));
  int line_goes_left = line.p2.x < line.p1.x ? 1 : -1;

  return point_above_line * line_goes_left;
}

bool point_in_polygon(point_t point, polygon_t polygon) {
  for (int i=1; i < polygon.num_points; i++) {
    if (side_of_line(point, line_t(polygon.points[i-1], polygon.points[i])) < 0) {
      return false;
    }
  }
  if (side_of_line(point, line_t(polygon.points[polygon.num_points-1], polygon.points[0])) < 0) {
    return false;
  }
  return true;
}