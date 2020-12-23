#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

inline constexpr std::size_t PreambleSize = 25;

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
    auto adapters = AoC::readVector<U64>(stream);
    adapters.push_back(0);
    adapters.push_back(*std::ranges::max_element(adapters) + 3);
    std::ranges::sort(adapters);
    if (part == 1) {
      std::array<decltype(adapters)::value_type, 4> differences{};
      AoC::mapOverAdjacentPairs(adapters, [&differences](auto const a, auto const b) { differences.at(b - a)++; });
      std::cout << differences[1] * differences[3] << '\n';
    } else {
      std::array<decltype(adapters)::value_type, 4> ways{};
      ways[0] = 1;
      auto i = adapters.size() - 1;
      while (i > 0) {
        i--;
        std::shift_right(std::begin(ways), std::end(ways), 1);
        ways[0] = 0;
        for (std::size_t d = 1; d <= 3 && i + d < adapters.size() && adapters[i + d] - adapters[i] <= 3; d++) {
          ways[0] += ways[d];
        }
      }
      std::cout << ways[0] << '\n';
    }
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
