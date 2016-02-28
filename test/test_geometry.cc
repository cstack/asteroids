#include "../engine/geometry.h"

void test_side_of_line(point_t p_test, point_t p1, point_t p2) {
  int result = side_of_line(p_test, p1, p2);
  std::cout << "side_of_line(" << p_test << ", " << p1 << ", " << p2 << ") -> " << result << std::endl;
}

int main() {
  point_t p_test;
  point_t p1;
  point_t p2;

  const uint NUM_VALUES = 3;
  double values[NUM_VALUES] = {-1, 0, 1};

  p1.x = 0;
  p1.y = 0;
  p2.x = 1;
  p2.y = 1;

  p_test.x = 0;
  p_test.y = 1;
  test_side_of_line(p_test, p1, p2);

  p_test.x = 1;
  p_test.y = 1;
  test_side_of_line(p_test, p1, p2);

  p_test.x = 1;
  p_test.y = 0;
  test_side_of_line(p_test, p1, p2);

  p_test.x = 0;
  p_test.y = -1;
  test_side_of_line(p_test, p1, p2);

  p1.x = 1;
  p1.y = 1;
  p2.x = 0;
  p2.y = 0;

  p_test.x = 0;
  p_test.y = 1;
  test_side_of_line(p_test, p1, p2);

  p_test.x = 1;
  p_test.y = 0;
  test_side_of_line(p_test, p1, p2);

  p1.x = 0;
  p1.y = 0;
  p2.x = 0;
  p2.y = 1;

  p_test.x = 0;
  p_test.y = 0;
  test_side_of_line(p_test, p1, p2);

  p_test.x = 1;
  p_test.y = 0;
  test_side_of_line(p_test, p1, p2);

  p_test.x = -1;
  p_test.y = 0;
  test_side_of_line(p_test, p1, p2);

  p1.x = 0;
  p1.y = 0;
  p2.x = 0;
  p2.y = -1;

  p_test.x = 0;
  p_test.y = 0;
  test_side_of_line(p_test, p1, p2);

  p_test.x = 1;
  p_test.y = 0;
  test_side_of_line(p_test, p1, p2);

  p_test.x = -1;
  p_test.y = 0;
  test_side_of_line(p_test, p1, p2);

  p1.x = 0;
  p1.y = 0;
  p2.x = 1;
  p2.y = 0;

  p_test.x = 0;
  p_test.y = 0;
  test_side_of_line(p_test, p1, p2);

  p_test.x = 0;
  p_test.y = 1;
  test_side_of_line(p_test, p1, p2);

  p_test.x = 0;
  p_test.y = -1;
  test_side_of_line(p_test, p1, p2);

  p_test.x = 0;
  p_test.y = 0;
  p1.x = 1;
  p1.y = 0;
  p2.x = 0;
  p2.y = -1;
  test_side_of_line(p_test, p1, p2);
}