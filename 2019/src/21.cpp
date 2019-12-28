#include "ArgumentParser.hpp"
#include "Intcode.hpp"
#include "Text.hpp"

#include <cassert>
#include <iostream>

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part != 1 && part != 2) {
    throw std::invalid_argument("Part should be 1 or 2.");
  }

  auto originalIntcode = Intcode(readMemory(path));
  const auto enterInstruction = [&originalIntcode](const std::string &string) {
    for (const auto c : string) {
      originalIntcode.addInput(c);
    }
    originalIntcode.addInput('\n');
  };
  if (part == 1) {
    enterInstruction("NOT A T");
    enterInstruction("OR T J");
    enterInstruction("NOT B T");
    enterInstruction("OR T J");
    enterInstruction("NOT C T");
    enterInstruction("OR T J");

    enterInstruction("AND D J");

    enterInstruction("WALK");
    const auto finalState = originalIntcode.run();
    assert(finalState == IntcodeState::Halted);
    while (originalIntcode.getOutputBufferLength() > 1) {
      std::cout << static_cast<char>(originalIntcode.getOutput());
    }
    std::cout << originalIntcode.getOutput() << '\n';
  } else {
  }
  return 0;
}
