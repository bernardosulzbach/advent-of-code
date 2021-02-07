#pragma once

#include "Direction.hpp"
#include "Interval.hpp"
#include "Math.hpp"
#include "Types.hpp"
#include "Vector.hpp"

template <typename T, Size DimensionCount> struct Point {
  using CoordinateType = T;
  static constexpr auto Dimensions = DimensionCount;

  static_assert(Dimensions >= 2);

  [[nodiscard]] constexpr Point() noexcept = default;

  [[nodiscard]] constexpr Point(T const x) noexcept : fromOrigin({x}) {
    static_assert(Dimensions == 1);
  }

  [[nodiscard]] constexpr Point(T const x, T const y) noexcept : fromOrigin({x, y}) {
    static_assert(Dimensions == 2);
  }

  [[nodiscard]] constexpr Point(T const x, T const y, T const z) noexcept : fromOrigin({x, y, z}) {
    static_assert(Dimensions == 3);
  }

  [[nodiscard]] constexpr Point(T const x, T const y, T const z, T const w) noexcept : fromOrigin({x, y, z, w}) {
    static_assert(Dimensions == 4);
  }

  [[nodiscard]] constexpr Point(Vector<T, Dimensions> const vector) noexcept : fromOrigin(vector){};

  [[nodiscard]] T &operator[](Size const i) &noexcept {
    return fromOrigin[i];
  }

  [[nodiscard]] T const &operator[](Size const i) const &noexcept {
    return fromOrigin[i];
  }

  [[nodiscard]] constexpr T const &getX() const noexcept {
    return (*this)[0];
  }

  [[nodiscard]] constexpr T &getX() noexcept {
    return (*this)[0];
  }

  [[nodiscard]] constexpr T const &getY() const noexcept {
    return (*this)[1];
  }

  [[nodiscard]] constexpr T &getY() noexcept {
    return (*this)[1];
  }

  [[nodiscard]] constexpr T const &getZ() const noexcept {
    static_assert(Dimensions >= 3);
    return (*this)[2];
  }

  [[nodiscard]] constexpr T &getZ() noexcept {
    static_assert(Dimensions >= 3);
    return (*this)[2];
  }

  using DirectNeighborsArray = std::array<Point, 2u * Dimensions>;
  [[nodiscard]] constexpr DirectNeighborsArray getDirectNeighbors() const noexcept {
    static_assert(std::is_integral_v<T>,
                  "This function offsets fromOrigin by 1. This doesn't make sense with floating point points.");
    static_assert(std::is_signed_v<T>, "This doesn't check for underflow, better avoid it with unsigned types.");
    DirectNeighborsArray neighbors;
    auto neighborsIterator = std::begin(neighbors);
    for (Size dimension = 0; dimension < Dimensions; dimension++) {
      static_assert(std::is_signed_v<T>,
                    "-1 is needed in the next line, fix this as well if you need this with unsigned integers.");
      for (auto const delta : {static_cast<T>(-1), static_cast<T>(1)}) {
        *neighborsIterator = *this;
        neighborsIterator->fromOrigin[dimension] += delta;
        ++neighborsIterator;
      }
    }
    assert(neighborsIterator == std::end(neighbors));
    return neighbors;
  }

  using NeighborsArray = std::array<Point, AoC::pow(3u, Dimensions) - 1u>;
  [[nodiscard]] constexpr NeighborsArray getNeighbors() const noexcept {
    static_assert(std::is_integral_v<T>,
                  "This function offsets fromOrigin by 1. This doesn't make sense with floating point points.");
    static_assert(std::is_signed_v<T>, "This doesn't check for underflow, better avoid it with unsigned types.");
    NeighborsArray neighbors;
    auto neighborsIterator = std::begin(neighbors);
    auto copy = *this;
    getNeighborsRecursion(neighborsIterator, copy, true, 0);
    assert(neighborsIterator == std::end(neighbors));
    return neighbors;
  }

  /**
   * Returns a vector that translates this Point to another point.
   */
  [[nodiscard]] constexpr Vector<T, Dimensions> to(const Point &rhs) const noexcept {
    return rhs.fromOrigin - fromOrigin;
  }

  /**
   * Returns a vector that translates this Point to the origin.
   */
  [[nodiscard]] constexpr Vector<T, Dimensions> toOrigin() const noexcept {
    return -fromOrigin;
  }

  [[nodiscard]] constexpr Point operator+(Vector<T, Dimensions> const vector) const noexcept {
    return Point{fromOrigin + vector};
  }

  [[nodiscard]] constexpr auto operator<=>(Point const &rhs) const noexcept = default;

  /**
   * Rotates the point around a pivot point the specified number of degrees. Positive angles are CCW rotations.
   */
  [[nodiscard]] constexpr Point rotateAround(Point const pivot, T const amount) const noexcept {
    static_assert(std::is_signed_v<T>, "Needs to be able to use negative angles.");
    static_assert(Dimensions == 2, "Only defined for 2-D points.");
    assert(amount % 90 == 0);
    assert((AoC::Interval<T>{-360, 360}.contains(amount)));
    auto const pivotToOrigin = pivot.toOrigin();
    auto const counterClockwise = amount > 0;
    auto rotatedPoint = (*this) + pivotToOrigin;
    for (auto rotations = std::abs(amount / 90) % 4; rotations > 0; rotations--) {
      if (counterClockwise) {
        rotatedPoint = Point{-rotatedPoint.getY(), rotatedPoint.getX()};
      } else {
        rotatedPoint = Point{rotatedPoint.getY(), -rotatedPoint.getX()};
      }
    }
    return rotatedPoint + pivotToOrigin.scale(-1);
  }

  [[nodiscard]] constexpr Point move(Direction const direction, T const amount = static_cast<T>(1)) const noexcept {
    static_assert(Dimensions == 2, "Only defined for 2-D points.");
    if (direction == Direction::North) {
      return Point{getX(), getY() + amount};
    }
    if (direction == Direction::East) {
      return Point{getX() + amount, getY()};
    }
    if (direction == Direction::South) {
      return Point{getX(), getY() - amount};
    }
    assert(direction == Direction::West);
    return Point{getX() - amount, getY()};
  }

  friend std::ostream &operator<<(std::ostream &os, Point const &point) noexcept {
    return os << point.fromOrigin;
  }

  [[nodiscard]] AoC::Hash hash() const noexcept {
    return fromOrigin.hash();
  }

private:
  Vector<T, Dimensions> fromOrigin; // Defined as a vector from the origin.

  template <typename NeighborArrayIterator>
  constexpr void getNeighborsRecursion(NeighborArrayIterator &neighborsIterator, Point &currentPoint,
                                       bool const equalToPoint, Size const dimension) const noexcept {
    if (dimension == Dimensions) {
      if (!equalToPoint) {
        *neighborsIterator = currentPoint;
        ++neighborsIterator;
      }
      return;
    }
    auto const initialValue = currentPoint[dimension];
    for (std::decay_t<decltype(initialValue)> value = initialValue - 1; value <= initialValue + 1; value++) {
      currentPoint[dimension] = value;
      getNeighborsRecursion(neighborsIterator, currentPoint, equalToPoint && (value == initialValue), dimension + 1);
    }
    currentPoint[dimension] = initialValue;
  }
};

template <typename T, Size Dimensions> struct std::hash<Point<T, Dimensions>> {
  std::size_t operator()(Point<T, Dimensions> const &point) const {
    return point.hash().toStandardSize();
  }
};
