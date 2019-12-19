#include "ArgumentParser.hpp"
#include "Body.hpp"
#include "Cycle.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Point.hpp"
#include "Robot.hpp"
#include "Screen.hpp"
#include "Vector.hpp"

#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <optional>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

constexpr auto EmptySpaceSymbol = ' ';
constexpr auto WallSymbol = '+';
constexpr auto BlockSymbol = '*';
constexpr auto PadSymbol = '-';
constexpr auto BallSymbol = 'o';

char characterFromTileId(U64 tileId) {
  switch (tileId) {
  case 0:
    return EmptySpaceSymbol;
  case 1:
    return WallSymbol;
  case 2:
    return BlockSymbol;
  case 3:
    return PadSymbol;
  case 4:
    return BallSymbol;
  default:
    throw std::runtime_error("Should never happen.");
  }
}

void updateScreen(Intcode &intcode, Screen &screen) {
  while (intcode.hasOutput()) {
    const auto x = intcode.getOutput();
    assert(intcode.hasOutput());
    const auto y = intcode.getOutput();
    assert(intcode.hasOutput());
    const auto value = intcode.getOutput();
    if (x >= 0 && y >= 0) {
      screen.paint(x, y, characterFromTileId(value));
    } else {
      assert(x == -1);
      assert(y == 0);
      screen.setScore(value);
    }
  }
}

struct DefeatCause {
  Point<S64> position;
  U32 turns;
};

DefeatCause findDefeatCause(Intcode intcode, Screen screen) {
  intcode.addInput(0);
  U32 turns = 1;
  while (intcode.run() != IntcodeState::Halted) {
    updateScreen(intcode, screen);
    intcode.addInput(0);
    turns++;
  }
  return DefeatCause{screen.getPosition(BallSymbol), turns};
}

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part != 1 && part != 2) {
    throw std::invalid_argument("Part should be 1 or 2.");
  }
  Intcode intcode(readMemory(path));
  Screen screen;
  if (part == 1) {
    intcode.run();
    updateScreen(intcode, screen);
    const auto string = screen.toString();
    std::cout << std::count(std::begin(string), std::end(string), BlockSymbol) << '\n';
  } else {
    intcode.memory[0] = 2;
    std::optional<Point<S64>> previousBallPosition{};
    while (intcode.run() != IntcodeState::Halted) {
      updateScreen(intcode, screen);
      // Idea: find the position at which we are defeated, then move the paddle to that position in the old state.
      const auto defeatCause = findDefeatCause(intcode, screen);
      const auto currentPaddlePosition = screen.getPosition(PadSymbol);
      const auto dx = defeatCause.position.x - currentPaddlePosition.x;
      const auto currentBallPosition = screen.getPosition(BallSymbol);
      if (std::abs(dx) + 2 >= defeatCause.turns) {
        if (currentPaddlePosition.x == defeatCause.position.x) {
          intcode.addInput(0);
        } else if (currentPaddlePosition.x < defeatCause.position.x) {
          intcode.addInput(+1);
        } else {
          intcode.addInput(-1);
        }
      } else {
        intcode.addInput(0);
      }
      std::cout << screen.toString() << '\n';
      previousBallPosition = currentBallPosition;
    }
  }
  return 0;
}
