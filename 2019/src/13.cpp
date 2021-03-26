#include "ArgumentParser.hpp"
#include "Body.hpp"
#include "Cycle.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Point.hpp"
#include "Screen.hpp"
#include "Vector.hpp"

#include <cassert>
#include <chrono>

#include <iostream>
#include <optional>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace AoC {
constexpr auto EmptySpaceSymbol = ' ';
constexpr auto WallSymbol = '+';
constexpr auto BlockSymbol = '*';
constexpr auto PadSymbol = '-';
constexpr auto BallSymbol = 'o';

[[nodiscard]] char characterFromTileId(Intcode::ValueType tileId) {
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
      screen.paint(unsignedCast<Unchecked>(x), unsignedCast<Unchecked>(y), characterFromTileId(value));
    } else {
      assert(x == -1);
      assert(y == 0);
      screen.setScore(value);
    }
  }
}

struct DefeatCause {
  Point<Size, 2> position;
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
  return {screen.getPosition(BallSymbol), turns};
}

void main(std::istream &stream, U32 const part) {
  Intcode intcode(readMemory(stream));
  Screen screen;
  if (part == 1) {
    intcode.run();
    updateScreen(intcode, screen);
    const auto string = screen.toString();
    std::cout << std::count(std::begin(string), std::end(string), BlockSymbol) << '\n';
  } else {
    intcode.memory[0] = 2;
    std::optional<Point<Size, 2>> previousBallPosition;
    while (intcode.run() != IntcodeState::Halted) {
      updateScreen(intcode, screen);
      // Idea: find the position at which we are defeated, then move the paddle to that position in the old state.
      const auto defeatCause = findDefeatCause(intcode, screen);
      const auto currentPaddlePosition = screen.getPosition(PadSymbol);
      const auto dx = checkedCast<S64>(defeatCause.position.getX()) - checkedCast<S64>(currentPaddlePosition.getX());
      const auto currentBallPosition = screen.getPosition(BallSymbol);
      if (std::abs(dx) + 2 >= defeatCause.turns) {
        if (currentPaddlePosition.getX() == defeatCause.position.getX()) {
          intcode.addInput(0);
        } else if (currentPaddlePosition.getX() < defeatCause.position.getX()) {
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
}
} // namespace AoC

#include "Main.inl"
