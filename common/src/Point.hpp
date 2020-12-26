#pragma once

#include <functional>

#include <boost/functional/hash/hash.hpp>

#include "Direction.hpp"
#include "Vector.hpp"

template <typename T> class Point {
public:
  T x{};
  T y{};

  [[nodiscard]] constexpr Point() noexcept = default;

  [[nodiscard]] constexpr Point(T const pointX, T const pointY) noexcept : x(pointX), y(pointY) {
  }

  [[nodiscard]] constexpr Vector<T, 2> subtract(const Point &rhs) const {
    return Vector<T, 2>{{x - rhs.x, y - rhs.y}};
  }

  [[nodiscard]] constexpr Point shift(Vector<T, 2> const vector) const {
    return Point(x + vector.components[0], y + vector.components[1]);
  }

  [[nodiscard]] constexpr bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  [[nodiscard]] constexpr bool operator!=(const Point &rhs) const {
    return !(rhs == *this);
  }

  /**
   * Rotates the point around a pivot point the specified number of degrees. Positive angles are CCW rotations.
   */
  [[nodiscard]] constexpr Point<T> rotateAround(Point<T> const pivot, T const amount) const {
    static_assert(std::is_signed_v<T>, "Needs to be able to use negative angles.");
    assert(amount % 90 == 0);
    auto const pivotToOrigin = Point<T>{}.subtract(pivot);
    auto const counterClockwise = amount > 0;
    auto rotatedPoint = shift(pivotToOrigin);
    for (auto rotations = std::abs(amount / 90) % 4; rotations > 0; rotations--) {
      if (counterClockwise) {
        rotatedPoint = Point<T>{-rotatedPoint.y, rotatedPoint.x};
      } else {
        rotatedPoint = Point<T>{rotatedPoint.y, -rotatedPoint.x};
      }
    }
    return rotatedPoint.shift(pivotToOrigin.scale(-1));
  }

  [[nodiscard]] constexpr Point<T> move(Direction direction, T amount = static_cast<T>(1)) const {
    switch (direction) {
    case Direction::North:
      return Point(x, y + amount);
    case Direction::East:
      return Point(x + amount, y);
    case Direction::South:
      return Point(x, y - amount);
    case Direction::West:
      return Point(x - amount, y);
    }
    throw std::runtime_error("Should be unreachable.");
  }
};

template <typename T> struct std::hash<Point<T>> {
  std::size_t operator()(const Point<T> &point) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, point.x);
    boost::hash_combine(seed, point.y);
    return seed;
  }
};
