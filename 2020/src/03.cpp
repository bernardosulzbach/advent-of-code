#include "ArgumentParser.hpp"
#include "Direction.hpp"
#include "IO.hpp"
#include "Point.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace AoC {
void main(ArgumentParser const &argumentParser) {
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
  if (argumentParser.getPart() == 1) {
    std::cout << countTrees(3, 1) << '\n';
  } else {
    std::cout << countTrees(1, 1) * countTrees(3, 1) * countTrees(5, 1) * countTrees(7, 1) * countTrees(1, 2) << '\n';
  }
}
} // namespace AoC

#include "Main.inl"