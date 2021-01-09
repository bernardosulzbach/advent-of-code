#pragma once

#include "Point.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

namespace AoC {
template <typename T, Size Dimensions> struct BoundingBox {
  static constexpr auto MaxValue = std::numeric_limits<T>::max();
  static constexpr auto MinValue = std::numeric_limits<T>::lowest();

  Point<T, Dimensions> min{MaxValue, MaxValue, MaxValue};
  Point<T, Dimensions> max{MinValue, MinValue, MinValue};

  template <typename PointRange> explicit BoundingBox(PointRange const &pointRange) noexcept {
    for (auto const point : pointRange) {
      for (Size dimension = 0; dimension < Dimensions; dimension++) {
        min[dimension] = std::min(min[dimension], point[dimension]);
        max[dimension] = std::max(max[dimension], point[dimension]);
      }
    }
  }
};

template <typename PointRange>
BoundingBox(PointRange const &pointRange)
    -> BoundingBox<typename RangeType<PointRange>::CoordinateType, RangeType<PointRange>::Dimensions>;
} // namespace AoC