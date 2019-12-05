#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Point.hpp"
#include "Types.hpp"

template <typename T> class Path {
  std::unordered_map<Point<T>, std::vector<U32>> visited;

public:
  Path() = default;

  void addPoint(Point<T> p, U32 t) {
    visited[p].push_back(t);
  }

  std::vector<U32> getVisits(const Point<T> point) const {
    if (visited.find(point) == visited.end()) {
      return {};
    }
    return visited.find(point)->second;
  }

  std::unordered_set<Point<T>> computeIntersection(const Path &rhs) const {
    std::unordered_set<Point<T>> intersection;
    for (const auto &value : visited) {
      if (rhs.visited.find(value.first) != rhs.visited.end()) {
        intersection.insert(value.first);
      }
    }
    return intersection;
  }
};
