#include "ArgumentParser.hpp"
#include "Intcode.hpp"

int main(int argc, char **argv) {
  const auto path = parseArguments(argc, argv);
  const auto memory = readMemory(path);
  {
    Intcode intcode(memory);
    intcode.addInput(1);
    intcode.run();
  }
  {
    Intcode intcode(memory);
    intcode.addInput(5);
    intcode.run();
  }
  return 0;
}
