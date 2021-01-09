#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <ranges>
#include <stdexcept>

#include "Hash.hpp"
#include "IO.hpp"
#include "Types.hpp"

template <typename T, Size N> struct Vector {
  std::array<T, N> components{};

  [[nodiscard]] Vector scale(const T t) const noexcept {
    auto copy = *this;
    std::ranges::transform(copy.components, std::ranges::begin(copy.components),
                           [t](auto const &value) { return t * value; });
    return copy;
  }

  [[nodiscard]] constexpr T getL1Norm() const noexcept {
    return std::accumulate(std::begin(components), std::end(components), T{},
                           [](auto const accumulated, auto const value) { return accumulated + std::abs(value); });
  }

  [[nodiscard]] constexpr T getL2NormSquare() const noexcept {
    return std::accumulate(std::begin(components), std::end(components), T{},
                           [](auto const accumulated, auto const value) { return accumulated + value * value; });
  }

  [[nodiscard]] constexpr F64 getL2Norm() const noexcept {
    return std::sqrt(getL2NormSquare());
  }

  [[nodiscard]] constexpr auto operator<=>(Vector const &rhs) const noexcept = default;

  [[nodiscard]] constexpr Vector operator-() const &noexcept {
    return scale(-1);
  }

  [[nodiscard]] constexpr Vector operator+(Vector const rhs) const &noexcept {
    auto copy = *this;
    copy += rhs;
    return copy;
  }

  constexpr Vector &operator+=(Vector const rhs) &noexcept {
    pairwise(rhs, [](const auto a, const auto b) { return a + b; });
    return *this;
  }

  [[nodiscard]] constexpr Vector operator-(Vector const rhs) const &noexcept {
    auto copy = *this;
    copy -= rhs;
    return copy;
  }

  constexpr Vector &operator-=(Vector const rhs) &noexcept {
    pairwise(rhs, [](const auto a, const auto b) { return a - b; });
    return *this;
  }

  [[nodiscard]] T &operator[](Size const i) &noexcept {
    return components[i];
  }

  [[nodiscard]] T const &operator[](Size const i) const &noexcept {
    return components[i];
  }

  friend std::ostream &operator<<(std::ostream &os, Vector const &vector) noexcept {
    os << "<";
    AoC::print(os, vector.components, ", ", false);
    return os << ">";
  }

  [[nodiscard]] Vector normalize() const {
    const auto norm = getL2Norm();
    if (norm == 0.0) {
      throw std::invalid_argument("Null vector cannot be normalized.");
    }
    return scale(1.0 / norm);
  }

  [[nodiscard]] AoC::Hash hash() const noexcept {
    return AoC::Hash(components);
  }

private:
  constexpr void pairwise(Vector const rhs, auto const &function) &noexcept {
    for (Size i = 0; i < N; i++) {
      (*this)[i] = function((*this)[i], rhs[i]);
    }
  }
};

inline constexpr std::array<Vector<S64, 2>, 8> DirectionVectors = {
    {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}}};

template <typename T, Size N> struct std::hash<Vector<T, N>> {
  std::size_t operator()(Vector<T, N> const &vector) const noexcept {
    return vector.hash().toStandardSize();
  }
};

[[nodiscard]] F64 getAngle(Vector<F64, 2> const vector) noexcept;