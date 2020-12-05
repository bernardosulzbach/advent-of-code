#pragma once

#include <cmath>

#include "Types.hpp"

constexpr F64 Pi = 3.141592653589793238462643383279502884;

template <typename T> T greatestCommonDivisor(T a, T b) {
  if (a == 0) {
    return b;
  }
  return greatestCommonDivisor(b % a, a);
}

template <typename T> T leastCommonMultiple(T a, T b) {
  return (a * b) / greatestCommonDivisor(a, b);
}