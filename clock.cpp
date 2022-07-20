#include <api.h>

#include "draw_circle.hpp"
#include "trig.hpp"

int main() {
  api_init();
  unsigned char angle = 0;
  CLEAR();
  draw_circle(64, 32, 31);
  for (;;) {
    DRAW_LINE(64, 32, 64 + cos(angle, 30), 32 - sin(angle, 30), 0);
    --angle;
    DRAW_LINE(64, 32, 64 + cos(angle, 30), 32 - sin(angle, 30), 1);
    DISPLAY();
    WAIT();
  }
}