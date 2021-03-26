#include <cassert>
#include <fstream>

static_assert(std::is_same<decltype(AoC::main(std::declval<std::ifstream &>(), std::declval<U32>())), void>::value,
              "AoC::main should return void.");

int main(int argc, char **argv) {
  try {
    AoC::ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    std::ifstream stream(argumentParser.getPath());
    assert(stream);
    AoC::main(stream, argumentParser.getPart());
  } catch (const std::exception &exception) {
    std::cout << "Threw an exception:\n\t" << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
