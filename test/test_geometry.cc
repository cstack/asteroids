#include "../engine/geometry.h"

void test_side_of_line(point_t p, line_t line) {
  int result = side_of_line(p, line);
  std::cout << "side_of_line(" << p << ", " << line << ") -> " << result << std::endl;
}

void test_point_in_polygon(point_t point, polygon_t polygon) {
  bool result = point_in_polygon(point, polygon);
  std::cout << "point_in_polygon(" << point << ", " << polygon << ") -> " << result << std::endl;
}

int main() {
  point_t p_test;
  point_t p1;
  point_t p2;

  p1.x = 0;
  p1.y = 0;
  p2.x = 1;
  p2.y = 1;

  p_test.x = 0;
  p_test.y = 1;
  test_side_of_line(p_test, line_t(0, 0, 1, 1));

  p_test.x = 1;
  p_test.y = 1;
  test_side_of_line(p_test, line_t(0, 0, 1, 1));

  p_test.x = 1;
  p_test.y = 0;
  test_side_of_line(p_test, line_t(0, 0, 1, 1));

  p_test.x = 0;
  p_test.y = -1;
  test_side_of_line(p_test, line_t(0, 0, 1, 1));

  p1.x = 1;
  p1.y = 1;
  p2.x = 0;
  p2.y = 0;

  p_test.x = 0;
  p_test.y = 1;
  test_side_of_line(p_test, line_t(1, 1, 0, 0));

  p_test.x = 1;
  p_test.y = 0;
  test_side_of_line(p_test, line_t(1, 1, 0, 0));

  p1.x = 0;
  p1.y = 0;
  p2.x = 0;
  p2.y = 1;

  p_test.x = 0;
  p_test.y = 0;
  test_side_of_line(p_test, line_t(0, 0, 0, 1));

  p_test.x = 1;
  p_test.y = 0;
  test_side_of_line(p_test, line_t(0, 0, 0, 1));

  p_test.x = -1;
  p_test.y = 0;
  test_side_of_line(p_test, line_t(0, 0, 0, 1));

  p1.x = 0;
  p1.y = 0;
  p2.x = 0;
  p2.y = -1;

  p_test.x = 0;
  p_test.y = 0;
  test_side_of_line(p_test, line_t(0, 0, 0, -1));

  p_test.x = 1;
  p_test.y = 0;
  test_side_of_line(p_test, line_t(0, 0, 0, -1));

  p_test.x = -1;
  p_test.y = 0;
  test_side_of_line(p_test, line_t(0, 0, 0, -1));

  p1.x = 0;
  p1.y = 0;
  p2.x = 1;
  p2.y = 0;

  p_test.x = 0;
  p_test.y = 0;
  test_side_of_line(p_test, line_t(0, 0, 1, 0));

  p_test.x = 0;
  p_test.y = 1;
  test_side_of_line(p_test, line_t(0, 0, 1, 0));

  p_test.x = 0;
  p_test.y = -1;
  test_side_of_line(p_test, line_t(0, 0, 1, 0));

  p_test.x = 0;
  p_test.y = 0;
  p1.x = 1;
  p1.y = 0;
  p2.x = 0;
  p2.y = -1;
  test_side_of_line(p_test, line_t(1, 0, 0, -1));

  p_test.x = 0.5;
  p_test.y = 0.6;
  p1.x = 0;
  p1.y = 1;
  p2.x = 1;
  p2.y = 0;
  test_side_of_line(p_test, line_t(0, 1, 1, 0));

  point_t point;
  polygon_t polygon;

  polygon.num_points = 3;
  polygon.points[0].x = 1;
  polygon.points[0].y = 0;
  polygon.points[1].x = 0;
  polygon.points[1].y = -1;
  polygon.points[2].x = 0;
  polygon.points[2].y = 1;

  point.x = 0;
  point.y = 0;
  test_point_in_polygon(point, polygon);

  point.x = 1;
  point.y = 0;
  test_point_in_polygon(point, polygon);

  point.x = 2;
  point.y = 0;
  test_point_in_polygon(point, polygon);

  point.x = 0.5;
  point.y = 0.5;
  test_point_in_polygon(point, polygon);

  point.x = 0.5;
  point.y = 0.6;
  test_point_in_polygon(point, polygon);
}