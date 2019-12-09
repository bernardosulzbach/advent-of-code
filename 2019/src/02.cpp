#include <fstream>
#include <iostream>
#include <vector>

#include "ArgumentParser.hpp"
#include "Intcode.hpp"

void partOne(const std::string &path) {
  auto memory = readMemory(path);
  memory[1] = 12;
  memory[2] = 2;
  Intcode intcode(memory);
  intcode.run();
  std::cout << intcode.memory[0] << '\n';
}

void partTwo(const std::string &path) {
  std::ifstream stream(path);
  std::vector<int> memory;
  int value;
  while (stream >> value) {
    memory.push_back(value);
    char comma;
    stream >> comma;
  }
  const auto originalMemory = memory;
  auto done = false;
  for (int noun = 0; !done && noun < 100; noun++) {
    for (int verb = 0; !done && verb < 100; verb++) {
      memory = originalMemory;
      memory[1] = noun;
      memory[2] = verb;
      Intcode intcode(memory);
      intcode.run();
      if (intcode.memory[0] == 19690720) {
        std::cout << 100 * noun + verb << '\n';
        done = true;
      }
    }
  }
}

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part == 1) {
    partOne(argumentParser.getPath());
  } else if (part == 2) {
    partTwo(argumentParser.getPath());
  } else {
    throw std::runtime_error("Part should be 1 or 2.");
  }
  return 0;
}
