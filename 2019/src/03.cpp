#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "ArgumentParser.hpp"
#include "Path.hpp"
#include "Point.hpp"
#include "Types.hpp"

Path<S32> pathFromString(const std::string &string) {
  Path<S32> path;
  Point<S32> lastPoint(0, 0);
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
        lastPoint.y++;
        break;
      case 'R':
        lastPoint.x++;
        break;
      case 'D':
        lastPoint.y--;
        break;
      case 'L':
        lastPoint.x--;
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

S32 distance(Point<S32> point) {
  return std::abs(point.x) + std::abs(point.y);
}

void solve(const std::string &inputPath, int part) {
  std::ifstream stream(inputPath);
  std::string pathStringA;
  std::string pathStringB;
  stream >> pathStringA >> pathStringB;
  const auto pathA = pathFromString(pathStringA);
  const auto pathB = pathFromString(pathStringB);
  if (part == 1) {
    std::optional<Point<S32>> nearestIntersection;
    for (const auto point : pathA.computeIntersection(pathB)) {
      if (point == Point<S32>(0, 0)) {
        continue;
      }
      if (!nearestIntersection || distance(point) < distance(nearestIntersection.value())) {
        nearestIntersection = point;
      }
    }
    std::cout << distance(nearestIntersection.value()) << '\n';
  } else if (part == 2) {
    std::optional<Point<S32>> nearestIntersection;
    auto nearestIntersectionCost = std::numeric_limits<U32>::max();
    for (const auto point : pathA.computeIntersection(pathB)) {
      if (point == Point<S32>(0, 0)) {
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

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part == 1 || part == 2) {
    solve(argumentParser.getPath(), part);
  } else {
    throw std::runtime_error("Part should be 1 or 2.");
  }
  return 0;
}
