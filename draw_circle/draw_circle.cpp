#include <api.h>
#include <inttypes.h>

#include "draw_circle.hpp"

// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm

void draw_circle(unsigned char x_, unsigned char y_, unsigned char r_) {
  int8_t x = r_;
  int8_t y = 0;
  int8_t e = 3 - 2 * r_;
  while (x >= y) {
    SET_PIXEL(x_ + x, y_ - y, 1);
    SET_PIXEL(x_ - x, y_ - y, 1);
    SET_PIXEL(x_ + x, y_ + y, 1);
    SET_PIXEL(x_ - x, y_ + y, 1);
    SET_PIXEL(x_ - y, y_ + x, 1);
    SET_PIXEL(x_ - y, y_ - x, 1);
    SET_PIXEL(x_ + y, y_ + x, 1);
    SET_PIXEL(x_ + y, y_ - x, 1);
    if (e > 0) {
      e = e + 2 * (5 - 2 * x + 2 * y);
      x -= 1;
    } else {
      e = e + 2 * (3 + 2 * y);
    }
    y += 1;
  }
}