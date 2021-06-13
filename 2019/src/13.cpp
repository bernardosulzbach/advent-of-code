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
      auto const ballPosition = screen.getPosition(BallSymbol);
      if (previousBallPosition && previousBallPosition->getX() == screen.getPosition(PadSymbol).getX()) {
        intcode.addInput(signedCast<Unchecked>(ballPosition.getX()) -
                         signedCast<Unchecked>(previousBallPosition->getX()));
      } else {
        intcode.addInput(0);
      }
      previousBallPosition = ballPosition;
    }
    updateScreen(intcode, screen);
    std::cout << screen.getScore() << '\n';
  }
}
} // namespace AoC

#include "Main.inl"
