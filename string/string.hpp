#pragma once

#include <stdint.h>
#include <string.h>
#undef byte
#include <type_traits>
#define byte unsigned char

namespace string {

namespace detail {
uint8_t itoa(uint32_t n);
extern char itoaBuf[11];

template <int kLen> struct String {
  explicit constexpr String(const char *s) : s(s) {}
  static constexpr int max_length = kLen;
  void write(char *&buf) const {
    memcpy(buf, s, kLen);
    buf += kLen;
  }

private:
  const char *s;
};
template <int kLen> constexpr String<kLen - 1> term(const char (&array)[kLen]) {
  return String<kLen - 1>{array};
}

template <class IntType> struct Int {
  static constexpr bool kSigned = std::is_signed_v<IntType>;
  explicit constexpr Int(IntType v) : v(v) {}
  static constexpr int max_length = 3 * sizeof(IntType) + (kSigned ? 1 : 0);
  void write(char *&buf) const {
    std::make_unsigned_t<IntType> local = v;
    if constexpr (kSigned) {
      if (v < 0) {
        *buf++ = '-';
        local = ~local + 1;
      }
    }
    uint8_t len = 0;
    for (int i = 10; i >= 0; --i) {
      ++len;
      itoaBuf[i] = '0' + (local % 10);
      local /= 10;
      if (!local) {
        break;
      }
    }
    memcpy(buf, itoaBuf + 11 - len, len);
    buf += len;
  }

private:
  IntType v;
};

template <class IntType, class = std::enable_if_t<std::is_integral_v<IntType>>>
constexpr Int<IntType> term(IntType s) {
  return Int<IntType>{s};
}

} // namespace detail

// Return a null-terminated string that's the result of formatting and
// concatenating all arguments. The lifetime of the string ends upon the next
// call to concat.
template <class... Ts> const char *concat(Ts &&...ts) {
  static char result[(decltype(detail::term(ts))::max_length + ...) + 1];
  char *buf = result;
  (detail::term(ts).write(buf), ...);
  *buf = 0;
  return result;
}

} // namespace string