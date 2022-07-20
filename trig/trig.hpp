#pragma once

#include <inttypes.h>

int sin(uint8_t angle, uint8_t radius);

inline int cos(uint8_t angle, uint8_t radius) {
  return sin(64 - angle, radius);
}