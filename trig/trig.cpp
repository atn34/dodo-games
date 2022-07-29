#include "trig.hpp"

namespace {
constexpr int kTaylorSeriesIterations = 15;

constexpr double factorial(double n) {
  if (n == 0) {
    return 1;
  } else {
    return n * factorial(n - 1);
  }
}

constexpr double pow(double x, double p) {
  if (p == 0) {
    return 1;
  } else {
    return x * pow(x, p - 1);
  }
}

constexpr double sin(double x) {
  const int iterations = kTaylorSeriesIterations;
  double result = 0;
  for (int n = 0; n < iterations; ++n) {
    result += pow(-1, n) / factorial(2 * n + 1) * pow(x, 2 * n + 1);
  }
  return result;
}

constexpr int8_t to_table(double x) { return int8_t(sin(x) * 127.f); }

constexpr double tau = 2 * 3.14159265;

struct SinTable {
  constexpr SinTable() : table() {
    for (int i = 0; i < 256; ++i) {
      table[i] = to_table(tau * double(i) / 256.0);
    }
  }

  int8_t table[256];
};

constexpr SinTable table;

} // namespace

int8_t sin(uint8_t angle, uint8_t radius) {
  return (int(radius) * table.table[angle]) / 127;
}
int8_t cos(uint8_t angle, uint8_t radius) { return sin(64 - angle, radius); }