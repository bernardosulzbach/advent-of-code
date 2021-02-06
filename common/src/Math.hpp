#pragma once

#include <bit>
#include <cmath>
#include <execution>
#include <iostream>
#include <numeric>
#include <tuple>

#include <boost/numeric/conversion/cast.hpp>

#include "Concepts.hpp"
#include "Ranges.hpp"
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

namespace AoC {
/**
 * Checks if the cast preserves the value before performing it.
 */
template <Integral Target, Integral Source> constexpr Target checkedCast(Source const source) {
  return boost::numeric_cast<Target>(source);
}

/**
 * Also checks if the cast preserves the value before performing it.
 */
template <Integral Source> constexpr std::make_unsigned_t<Source> unsignedCast(Source const source) {
  return boost::numeric_cast<std::make_unsigned_t<Source>>(source);
}

template <Integral ResultType, UnsignedIntegral ExponentType>
constexpr ResultType pow(ResultType const base, ExponentType const exponent, ResultType const result = 1) {
  return exponent == 0 ? result : pow(base * base, exponent / 2, (exponent % 2) ? result * base : result);
}

/**
 * Computes a % b, but the result is never negative.
 */
template <typename T> [[nodiscard]] T euclideanModulo(T const a, T const b) noexcept {
  auto const m = a % b;
  if (m < 0) {
    if (b < 0) {
      return m - b;
    }
    return m + b;
  }
  return m;
}

template <typename T> [[nodiscard]] T multiplicativeInverse(T a, T b) noexcept {
  auto b0 = b;
  T x0 = 0;
  T x1 = 1;
  if (b == 1) {
    return 1;
  }
  while (a > 1) {
    auto const q = a / b;
    a = std::exchange(b, a % b);
    x1 = std::exchange(x0, x1 - q * x0);
  }
  if (x1 < 0) {
    x1 += b0;
  }
  return x1;
}

/**
 * Chinese Remainder Theorem
 *
 * Given pairwise coprime positive integers in the vector n and arbitrary integers in the vector a, the system of
 * simultaneous congruences
 *
 *   x ≡ a[0]            (mod n[0])
 *   ...
 *   x ≡ a[n.size() - 1] (mod n[n.size() - 1])
 *
 * has a solution, and the solution is unique modulo N = n[0] * ... * n[n.size() - 1].
 *
 * This function finds this unique solution.
 */
template <typename T> [[nodiscard]] T solveCongruenceSystem(std::vector<T> const &n, std::vector<T> const &a) noexcept {
  auto const prod = AoC::product(n);
  T sm = 0;
  for (std::size_t i = 0; i < n.size(); i++) {
    auto const p = prod / n[i];
    sm += a[i] * multiplicativeInverse(p, n[i]) * p;
  }
  return sm % prod;
}

template <typename T> [[nodiscard]] constexpr bool isPowerOfTwo(T const t) noexcept {
  static_assert(std::is_integral_v<T>);
  return t >= 1 && std::popcount(t) == 1;
}
} // namespace AoC
