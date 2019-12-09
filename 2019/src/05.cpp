#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <iostream>

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  const auto memory = readMemory(path);
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part == 1) {
    Intcode intcode(memory);
    intcode.addInput(1);
    intcode.run();
    while (intcode.hasOutput()) {
      std::cout << intcode.getOutput();
      if (intcode.hasOutput()) {
        std::cout << ",";
      }
    }
    std::cout << '\n';
  } else if (part == 2) {
    Intcode intcode(memory);
    intcode.addInput(5);
    intcode.run();
    while (intcode.hasOutput()) {
      std::cout << intcode.getOutput();
      if (intcode.hasOutput()) {
        std::cout << ",";
      }
    }
    std::cout << '\n';
  } else {
    throw std::runtime_error("Part should be 1 or 2.");
  }
  return 0;
}
