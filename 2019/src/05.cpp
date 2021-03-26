#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <iostream>

namespace AoC {
void main(std::istream &stream, U32 const part) {
  const auto memory = readMemory(stream);
  Intcode intcode(memory);
  if (part == 1) {
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
