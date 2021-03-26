#include "ArgumentParser.hpp"
#include "Path.hpp"
#include "Point.hpp"
#include "Types.hpp"

#include <cmath>

#include <iostream>
#include <sstream>
#include <vector>

namespace AoC {
Path<S32> pathFromString(std::string &string) {
  Path<S32> path;
  Point<S32, 2> lastPoint{};
  U32 t = 0;
  path.addPoint(lastPoint, t++);
  std::stringstream stream(string);
  char direction = '\0';
  while (stream >> direction) {
    S32 amount = 0;
    stream >> amount;
    for (S32 i = 0; i < amount; i++) {
      switch (direction) {
      case 'U':
        lastPoint.getY()++;
        break;
      case 'R':
        lastPoint.getX()++;
        break;
      case 'D':
        lastPoint.getY()--;
        break;
      case 'L':
        lastPoint.getX()--;
        break;
      default:
        std::cerr << "Unknown direction: '" << direction << "'." << '\n';
      }
      path.addPoint(lastPoint, t++);
    }
    char comma = '\0';
    stream >> comma;
  }
  return path;
}

void main(std::istream &stream, U32 const part) {
  std::string pathStringA;
  std::string pathStringB;
  stream >> pathStringA >> pathStringB;
  const auto pathA = pathFromString(pathStringA);
  const auto pathB = pathFromString(pathStringB);
  if (part == 1) {
    std::optional<Point<S32, 2>> nearestIntersection;
    for (const auto point : pathA.computeIntersection(pathB)) {
      if (point == Point<S32, 2>(0, 0)) {
        continue;
      }
      if (!nearestIntersection || point.toOrigin().getL1Norm() < nearestIntersection->toOrigin().getL1Norm()) {
        nearestIntersection = point;
      }
    }
    std::cout << nearestIntersection->toOrigin().getL1Norm() << '\n';
  } else {
    std::optional<Point<S32, 2>> nearestIntersection;
    auto nearestIntersectionCost = std::numeric_limits<U32>::max();
    for (const auto point : pathA.computeIntersection(pathB)) {
      if (point == Point<S32, 2>(0, 0)) {
        continue;
      }
      if (!nearestIntersection || pathA.getVisits(point)[0] + pathB.getVisits(point)[0] < nearestIntersectionCost) {
        nearestIntersection = point;
        nearestIntersectionCost = pathA.getVisits(point)[0] + pathB.getVisits(point)[0];
      }
    }
    std::cout << nearestIntersectionCost << '\n';
  }
}
} // namespace AoC

#include "Main.inl"
