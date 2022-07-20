#pragma once

#include <inttypes.h>

template <int kShiftAmount, class IntType> struct FixedPoint {

  explicit constexpr FixedPoint(double x)
      : shiftedAmount(IntType((IntType(1) << kShiftAmount) * x)) {}

  FixedPoint() : shiftedAmount(0) {}

  FixedPoint<kShiftAmount, IntType>
  operator+(FixedPoint<kShiftAmount, IntType> rhs) const {
    FixedPoint result;
    result.shiftedAmount = shiftedAmount + rhs.shiftedAmount;
    return result;
  }

  FixedPoint<kShiftAmount, IntType>
  operator-(FixedPoint<kShiftAmount, IntType> rhs) const {
    FixedPoint result;
    result.shiftedAmount = shiftedAmount - rhs.shiftedAmount;
    return result;
  }

  FixedPoint<kShiftAmount, IntType>
  operator*(FixedPoint<kShiftAmount, IntType> rhs) const {
    FixedPoint result;
    result.shiftedAmount = (shiftedAmount * rhs.shiftedAmount) >> kShiftAmount;
    return result;
  }

  FixedPoint<kShiftAmount, IntType>
  operator/(FixedPoint<kShiftAmount, IntType> rhs) const {
    FixedPoint result;
    result.shiftedAmount = (shiftedAmount << kShiftAmount) / rhs.shiftedAmount;
    return result;
  }

  IntType asInt() const { return shiftedAmount >> kShiftAmount; }

  FixedPoint<kShiftAmount, IntType>
  operator+=(FixedPoint<kShiftAmount, IntType> rhs) {
    shiftedAmount += rhs.shiftedAmount;
    return *this;
  }

  FixedPoint<kShiftAmount, IntType>
  operator-=(FixedPoint<kShiftAmount, IntType> rhs) const {
    shiftedAmount -= rhs.shiftedAmount;
    *this;
  }

private:
  IntType shiftedAmount;
};
