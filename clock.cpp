#include <api.h>

#include "draw_circle.hpp"
#include "trig.hpp"

constexpr auto clockRadius = 31;
constexpr auto secondsRadius = 28;
constexpr auto minutesRadius = 25;
constexpr auto hoursRadius = 20;
constexpr auto clockCenterX = 64;
constexpr auto clockCenterY = 32;

uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;

void redraw() {
  uint8_t secondsAngle = 256 / 4 - int(seconds) * 64 / 15;
  uint8_t minutesAngle = 256 / 4 - int(minutes) * 64 / 15;
  uint8_t hoursAngle = 256 / 4 - int(hours) * 64 / 3;
  CLEAR();
  draw_circle(clockCenterX, clockCenterY, clockRadius);
  DRAW_LINE(clockCenterX, clockCenterY, 64 + cos(secondsAngle, secondsRadius),
            32 - sin(secondsAngle, secondsRadius), 1);
  DRAW_LINE(clockCenterX, clockCenterY, 64 + cos(minutesAngle, minutesRadius),
            32 - sin(minutesAngle, minutesRadius), 1);
  DRAW_LINE(clockCenterX, clockCenterY, 64 + cos(hoursAngle, hoursRadius),
            32 - sin(hoursAngle, hoursRadius), 1);
  DISPLAY();
}

int main() {
  api_init();
  CLEAR();
  uint8_t frames = 0;

  redraw();

  for (;;) {
    ++frames;
    if (frames == 20) {
      frames = 0;
      ++seconds;
      if (seconds == 60) {
        seconds = 0;
        ++minutes;
        if (minutes == 60) {
          minutes = 0;
          ++hours;
          if (hours == 12) {
            hours = 0;
          }
        }
      }
      redraw();
    }
    WAIT();
  }
}