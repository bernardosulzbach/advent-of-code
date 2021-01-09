#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Point.hpp"
#include "Types.hpp"

template <typename T> class Path {
  std::unordered_map<Point<T, 2>, std::vector<U32>> visited;

public:
  void addPoint(Point<T, 2> const p, U32 const t) noexcept {
    visited[p].push_back(t);
  }

  std::vector<U32> getVisits(Point<T, 2> const point) const noexcept {
    const auto iterator = visited.find(point);
    if (iterator == visited.end()) {
      return {};
    }
    return iterator->second;
  }

  std::unordered_set<Point<T, 2>> computeIntersection(Path const &rhs) const noexcept {
    std::unordered_set<Point<T, 2>> intersection;
    for (const auto &value : visited) {
      if (rhs.visited.find(value.first) != rhs.visited.end()) {
        intersection.insert(value.first);
      }
    }
    return intersection;
  }
};
