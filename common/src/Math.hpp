#pragma once

#include <cmath>

#include "Types.hpp"

constexpr F64 Pi = 3.141592653589793238462643383279502884;

template <typename T> T greatestCommonDivisor(T a, T b) {
  while (b != 0) {
    std::tie(a, b) = std::make_tuple(b, a % b);
  }
  return a;
}

template <typename T> T leastCommonMultiple(T a, T b) {
  return (a * b) / greatestCommonDivisor(a, b);
}

template <typename T> struct Interval {
  Interval(T const intervalMin, T const intervalMax) noexcept : min(intervalMin), max(intervalMax) {
  }

  [[nodiscard]] bool contains(T const value) const noexcept {
    return min <= value && value <= max;
  }

private:
  T min;
  T max;
};