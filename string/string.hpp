#pragma once

#include <stdint.h>
#include <string.h>
#undef byte
#include <type_traits>
#define byte unsigned char

namespace string {

namespace detail {
const char *itoa(uint32_t n);

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

template <bool kSigned,
          class IntType = std::conditional_t<kSigned, int32_t, uint32_t>>
struct Int {
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
    const char *formatted = itoa(local);
    const uint8_t len = strlen(formatted);
    memcpy(buf, formatted, len);
    buf += len;
  }

private:
  IntType v;
};

template <class IntType, class = std::enable_if_t<std::is_integral_v<IntType>>>
constexpr Int<std::is_signed_v<IntType>> term(IntType s) {
  return Int<std::is_signed_v<IntType>>{s};
}

template <class L, class R> struct Concat {
  constexpr Concat(L l, R r) : l(l), r(r) {}
  static constexpr int max_length = L::max_length + R::max_length;
  constexpr void write(char *&buf) const {
    l.write(buf);
    r.write(buf);
  }

private:
  L l;
  R r;
};

template <class L, class R> constexpr Concat<L, R> operator+(L l, R r) {
  return Concat{l, r};
}
} // namespace detail

// Return a null-terminated string that's the result of formatting and
// concatenating all arguments. The lifetime of the string ends upon the next
// call to concat.
template <class... Ts> const char *concat(Ts &&...ts) {
  auto expr = (detail::term(ts) + ...);
  static char result[decltype(expr)::max_length + 1];
  char *buf = result;
  expr.write(buf);
  *buf = 0;
  return result;
}

} // namespace string