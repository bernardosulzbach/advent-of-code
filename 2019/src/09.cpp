#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <iostream>

namespace AoC {
void main(std::istream &stream, U32 const part) {
  Intcode intcode(readMemory(stream));
  intcode.addInput(part);
  intcode.run();
  while (intcode.hasOutput()) {
    std::cout << intcode.getOutput() << '\n';
  }
}
} // namespace AoC

#include "Main.inl"
