#include "ArgumentParser.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Point.hpp"
#include "Pose.hpp"
#include "Vector.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace AoC {
void main(ArgumentParser const &argumentParser) {
  Intcode intcode(readMemory(argumentParser.getPath()));
  Pose robot;
  std::unordered_map<Point<S32, 2>, bool> hull;
  std::unordered_set<Point<S32, 2>> painted;
  if (argumentParser.getPart() == 2) {
    hull[robot.getPosition()] = true;
  }
  while (intcode.run() != IntcodeState::Halted) {
    intcode.addInput(hull[robot.getPosition()] ? 1 : 0);
    intcode.run();
    const auto color = intcode.getOutput();
    const auto direction = intcode.getOutput();
    painted.insert(robot.getPosition());
    hull[robot.getPosition()] = color;
    if (direction == 0) {
      robot.rotateLeft();
    } else if (direction == 1) {
      robot.rotateRight();
    } else {
      std::cerr << "Should not get a direction other than 0 or 1." << '\n';
    }
    robot.goForward();
  }
  if (argumentParser.getPart() == 1) {
    std::cout << painted.size() << '\n';
  } else {
    auto minX = std::numeric_limits<S32>::max();
    auto maxX = std::numeric_limits<S32>::min();
    auto minY = std::numeric_limits<S32>::max();
    auto maxY = std::numeric_limits<S32>::min();
    for (const auto &point : painted) {
      minX = std::min(minX, point.getX());
      maxX = std::max(maxX, point.getX());
      minY = std::min(minY, point.getY());
      maxY = std::max(maxY, point.getY());
    }
    std::vector<std::string> lines(maxY - minY + 1, std::string(maxX - minX + 1, ' '));
    for (const auto &hullEntry : hull) {
      const auto i = maxY - hullEntry.first.getY();
      const auto j = hullEntry.first.getX() - minX;
      lines[i][j] = hullEntry.second ? 'X' : ' ';
    }
    for (const auto &line : lines) {
      std::cout << line << '\n';
    }
  }
}
} // namespace AoC

#include "Main.inl"
