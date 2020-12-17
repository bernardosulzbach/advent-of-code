#pragma once

#include <functional>

#include <boost/functional/hash/hash.hpp>

#include "Direction.hpp"

template <typename T> class Point {
public:
  T x;
  T y;

  [[nodiscard]] Point(T const pointX, T const pointY) noexcept : x(pointX), y(pointY) {
  }

  [[nodiscard]] bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  [[nodiscard]] bool operator!=(const Point &rhs) const {
    return !(rhs == *this);
  }

  [[nodiscard]] Point<T> move(Direction direction, T amount = static_cast<T>(1)) const {
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
