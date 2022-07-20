#include <api.h>

#include "angle.hpp"
#include "draw_circle.hpp"

int main() {
  api_init();
  unsigned char angleVal = 0;
  for (;;) {
    CLEAR();
    draw_circle(64, 32, 31);
    auto angle = Angle(angleVal);
    DRAW_LINE(64, 32, 64 + angle.cos(31), 32 - angle.sin(31), 1);
    DISPLAY();
    WAIT();
    ++angleVal;
  }
}