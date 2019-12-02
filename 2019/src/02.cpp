#include <fstream>
#include <iostream>
#include <vector>

#include "Intcode.hpp"

void partOne() {
  auto memory = readMemory("../input/02.txt");
  memory[1] = 12;
  memory[2] = 2;
  Intcode intcode(memory);
  // intcode.setDebugging(true);
  intcode.run();
  std::cout << intcode.memory[0] << '\n';
}

void partTwo() {
  std::ifstream stream("../input/02.txt");
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

int main() {
  partOne();
  partTwo();
  return 0;
}