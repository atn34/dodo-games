#pragma once

struct Angle {
  explicit Angle(unsigned char val) : val(val) {}

  int sin(int r);
  int cos(int r);

private:
  unsigned char val;
};