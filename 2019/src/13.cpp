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
#include <fstream>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

char characterFromTileId(U64 tileId) {
  switch (tileId) {
  case 0:
    return ' ';
  case 1:
    return '+';
  case 2:
    return '*';
  case 3:
    return '-';
  case 4:
    return 'o';
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
    std::cout << std::count(std::begin(string), std::end(string), '*') << '\n';
  } else {
    intcode.memory[0] = 2;
    while (intcode.run() != IntcodeState::Halted) {
      updateScreen(intcode, screen);
      std::cout << screen.toString() << '\n';
      Intcode::ValueType direction = 0;
      std::cout << "> ";
      std::cin >> direction;
      intcode.addInput(direction);
    }
  }
  return 0;
}
