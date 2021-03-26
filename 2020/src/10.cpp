#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

#include <algorithm>
#include <cassert>

#include <iostream>
#include <ranges>
#include <vector>

namespace AoC {
inline constexpr std::size_t PreambleSize = 25;

void main(std::istream &stream, U32 const part) {
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
}
} // namespace AoC

#include "Main.inl"
