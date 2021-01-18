#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <fstream>
#include <iostream>
#include <vector>

namespace AoC {
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
  std::vector<Intcode::ValueType> memory;
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

void main(ArgumentParser const &argumentParser) {
  if (argumentParser.getPart() == 1) {
    partOne(argumentParser.getPath());
  } else {
    partTwo(argumentParser.getPath());
  }
}
} // namespace AoC

#include "Main.inl"