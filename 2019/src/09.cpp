#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <iostream>

namespace AoC {
void main(ArgumentParser const &argumentParser) {
  Intcode intcode(readMemory(argumentParser.getPath()));
  intcode.addInput(argumentParser.getPart());
  intcode.run();
  while (intcode.hasOutput()) {
    std::cout << intcode.getOutput() << '\n';
  }
}
} // namespace AoC

#include "Main.inl"