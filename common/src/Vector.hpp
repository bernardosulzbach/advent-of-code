#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <ranges>
#include <stdexcept>

#include <boost/functional/hash/hash.hpp>

#include "Types.hpp"

template <typename T, std::size_t N> class Vector {
public:
  std::array<T, N> components{};

  [[nodiscard]] Vector add(const Vector &rhs) const;
  [[nodiscard]] Vector subtract(const Vector &rhs) const;
  [[nodiscard]] Vector scale(const T t) const noexcept {
    Vector copy = *this;
    std::ranges::transform(copy.components, std::ranges::begin(copy.components),
                           [t](auto const &value) { return t * value; });
    return copy;
  }

  [[nodiscard]] F64 getNorm() const;
  [[nodiscard]] F64 getL1Norm() const;

  [[nodiscard]] T &operator[](std::size_t const i) &noexcept {
    return components[i];
  }
  [[nodiscard]] T const &operator[](std::size_t const i) const &noexcept {
    return components[i];
  }

  [[nodiscard]] Vector normalize() const;
};

inline constexpr std::array<Vector<S64, 2>, 8> DirectionVectors = {
    {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}}};

template <typename T, std::size_t N> Vector<T, N> Vector<T, N>::add(const Vector<T, N> &rhs) const {
  Vector<T, N> result;
  for (std::size_t i = 0; i < N; i++) {
    result.components[i] = components[i] + rhs.components[i];
  }
  return result;
}

template <typename T, std::size_t N> Vector<T, N> Vector<T, N>::subtract(const Vector<T, N> &rhs) const {
  Vector<T, N> result;
  for (std::size_t i = 0; i < N; i++) {
    result.components[i] = components[i] - rhs.components[i];
  }
  return result;
}

template <typename T, std::size_t N> F64 Vector<T, N>::getNorm() const {
  T sumOfSquares{};
  for (std::size_t i = 0; i < N; i++) {
    sumOfSquares += components[i] * components[i];
  }
  return std::sqrt(sumOfSquares);
}

template <typename T, std::size_t N> F64 Vector<T, N>::getL1Norm() const {
  T total{};
  for (std::size_t i = 0; i < N; i++) {
    total += std::abs(components[i]);
  }
  return total;
}

template <typename T, std::size_t N> Vector<T, N> Vector<T, N>::normalize() const {
  const auto norm = getNorm();
  if (norm == 0.0) {
    throw std::invalid_argument("Null vector cannot be normalized.");
  }
  Vector<T, N> result;
  for (std::size_t i = 0; i < N; i++) {
    result.components[i] = components[i] / norm;
  }
  return result;
}

template <typename T, std::size_t N> struct std::hash<Vector<T, N>> {
  std::size_t operator()(const Vector<T, N> &vector) const {
    return boost::hash_range(vector.components.begin(), vector.components.end());
  }
};

F64 getAngle(Vector<F64, 2> vector);