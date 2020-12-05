#include "ArgumentParser.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>

#include "Ranges.hpp"
#include "StandardInput.hpp"

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    std::ifstream stream(argumentParser.getPath());
    auto const input = AoC::readVector<std::int64_t>(stream);
    if (part == 1) {
      for (auto const pair : AoC::allPairs(input)) {
        if (pair.first + pair.second == 2020) {
          std::cout << pair.first * pair.second << '\n';
          break;
        }
      }
    }
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
