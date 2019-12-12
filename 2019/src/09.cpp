#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <iostream>
#include <optional>

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  const auto part = argumentParser.getAdditionalArgument(0);
  Intcode intcode(readMemory(path));
  if (part == 1) {
    intcode.addInput(1);
  } else if (part == 2) {
    intcode.addInput(2);
  } else {
    throw std::invalid_argument("Part should be 1 or 2.");
  }
  intcode.run();
  while (intcode.hasOutput()) {
    std::cout << intcode.getOutput() << '\n';
  }
  return 0;
}
