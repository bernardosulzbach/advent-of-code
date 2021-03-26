#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <iostream>
#include <vector>

namespace AoC {
void partOne(std::istream &stream) {
  auto memory = readMemory(stream);
  memory[1] = 12;
  memory[2] = 2;
  Intcode intcode(memory);
  intcode.run();
  std::cout << intcode.memory[0] << '\n';
}

void partTwo(std::istream &stream) {
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

void main(std::istream &stream, U32 const part) {
  if (part == 1) {
    partOne(stream);
  } else {
    partTwo(stream);
  }
}
} // namespace AoC

#include "Main.inl"
