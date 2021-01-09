#include "ArgumentParser.hpp"
#include "Direction.hpp"
#include "IO.hpp"
#include "Point.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
    auto const lines = AoC::readVector<std::string>(stream);
    auto const countTrees = [&lines](std::size_t const right, std::size_t const down) {
      std::size_t trees = 0;
      Point<std::size_t, 2> point{0, 0};
      while (true) {
        point = point.move(Direction::Right, right).move(Direction::Up, down); // Up is positive.
        if (lines.size() <= point.getY()) {
          break;
        }
        point.getX() %= lines[point.getY()].size();
        if (lines[point.getY()][point.getX()] == '#') {
          trees++;
        }
      }
      return trees;
    };
    if (part == 1) {
      std::cout << countTrees(3, 1) << '\n';
    } else {
      std::cout << countTrees(1, 1) * countTrees(3, 1) * countTrees(5, 1) * countTrees(7, 1) * countTrees(1, 2) << '\n';
    }
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
