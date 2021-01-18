#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <iostream>

namespace AoC {
void main(ArgumentParser const &argumentParser) {
  const auto memory = readMemory(argumentParser.getPath());
  Intcode intcode(memory);
  if (argumentParser.getPart() == 1) {
    intcode.addInput(1);
  } else {
    intcode.addInput(5);
  }
  intcode.run();
  while (intcode.hasOutput()) {
    std::cout << intcode.getOutput();
    if (intcode.hasOutput()) {
      std::cout << ",";
    }
  }
  std::cout << '\n';
}
} // namespace AoC

#include "Main.inl"