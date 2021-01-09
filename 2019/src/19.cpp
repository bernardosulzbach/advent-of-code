#include "ArgumentParser.hpp"
#include "Intcode.hpp"
#include "Point.hpp"
#include "Text.hpp"

#include <cassert>
#include <iostream>
#include <optional>

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part != 1 && part != 2) {
    throw std::invalid_argument("Part should be 1 or 2.");
  }

  const auto originalIntcode = Intcode(readMemory(path));
  const auto test = [&](Intcode::ValueType x, Intcode::ValueType y) {
    auto intcode = originalIntcode;
    const auto firstState = intcode.run();
    assert(firstState == IntcodeState::Blocked);
    intcode.addInput(x);
    intcode.addInput(y);
    const auto secondState = intcode.run();
    assert(secondState == IntcodeState::Halted);
    assert(intcode.hasOutput());
    return intcode.getOutput();
  };
  if (part == 1) {
    const Intcode::ValueType size = 50;
    S32 count = 0;
    for (Intcode::ValueType i = 0; i < size; i++) {
      for (Intcode::ValueType j = 0; j < size; j++) {
        if (test(j, i) == 1) {
          count++;
        }
      }
    }
    std::cout << count << '\n';
  } else {
    // Empirically determined.
    const Intcode::ValueType size = 2000;
    std::vector<std::vector<bool>> grid(size, std::vector<bool>(size));
    for (Intcode::ValueType i = 0; i < size; i++) {
      for (Intcode::ValueType j = 0; j < size; j++) {
        if (test(j, i) == 1) {
          grid[i][j] = true;
        }
      }
    }
    std::vector<std::vector<U32>> lengthRight(size + 1, std::vector<U32>(size + 1));
    std::vector<std::vector<U32>> lengthDown(size + 1, std::vector<U32>(size + 1));
    for (Intcode::ValueType i = size - 1; i >= 0; i--) {
      for (Intcode::ValueType j = size - 1; j >= 0; j--) {
        if (grid[i][j]) {
          lengthRight[i][j] = lengthRight[i][j + 1] + 1;
          lengthDown[i][j] = lengthDown[i + 1][j] + 1;
        } else {
          lengthRight[i][j] = 0;
          lengthDown[i][j] = 0;
        }
      }
    }
    std::optional<Point<U32, 2>> closestPoint;
    const auto getPointDistanceSquare = [](std::optional<Point<U32, 2>> const &point) {
      if (!point) {
        return std::numeric_limits<U32>::max();
      }
      return point->toOrigin().getL2NormSquare();
    };
    for (Intcode::ValueType i = 0; i < size; i++) {
      for (Intcode::ValueType j = 0; j < size; j++) {
        if (lengthRight[i][j] >= 100 && lengthDown[i][j] >= 100) {
          const auto point = Point<U32, 2>(j, i);
          if (getPointDistanceSquare(point) < getPointDistanceSquare(closestPoint)) {
            closestPoint = point;
          }
        }
      }
    }
    std::cout << 10000 * closestPoint->getX() + closestPoint->getY() << '\n';
    assert(closestPoint.has_value());
  }
  return 0;
}
