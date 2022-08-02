#include "string.hpp"

namespace string {
namespace detail {

const char *itoa(uint32_t n) {
  static char buf[11];
  buf[10] = 0;
  for (int i = 9; i >= 0; --i) {
    buf[i] = '0' + (n % 10);
    n /= 10;
    if (!n) {
      return buf + i;
    }
  }
  return "?";
}

} // namespace detail
} // namespace string