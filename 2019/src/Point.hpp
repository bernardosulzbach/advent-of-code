#pragma once

#include <functional>

template <typename T> class Point {
public:
  T x;
  T y;

  Point(T pointX, T pointY) : x(pointX), y(pointY) {
  }

  bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  bool operator!=(const Point &rhs) const {
    return !(rhs == *this);
  }
};

template <typename T> struct std::hash<Point<T>> {
  std::size_t operator()(const Point<T> &point) const {
    std::size_t result = 17;
    result = 31 * result + std::hash<T>()(point.x);
    result = 31 * result + std::hash<T>()(point.y);
    return result;
  }
};
