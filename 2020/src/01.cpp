#include "ArgumentParser.hpp"
#include "Ranges.hpp"
#include "StandardInput.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>

template <std::size_t N, typename Range> void findCombination(Range const &input) {
  for (auto const combination : AoC::allCombinations<N>(input)) {
    if (AoC::sum(combination) == 2020) {
      std::cout << AoC::product(combination) << '\n';
      break;
    }
  }
}

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    std::ifstream stream(argumentParser.getPath());
    auto const input = AoC::readVector<std::int64_t>(stream);
    if (part == 1) {
      findCombination<2>(input);
    } else {
      findCombination<3>(input);
    }
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
