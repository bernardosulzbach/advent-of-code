#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace AoC {
inline constexpr std::size_t PreambleSize = 25;

void main(ArgumentParser const &argumentParser) {
  auto const part = argumentParser.getPart();
  auto stream = std::ifstream(argumentParser.getPath());
  auto const lastValues = AoC::readVector<U64>(stream);
  auto const firstViolation = [&lastValues]() {
    for (auto i = PreambleSize; i < lastValues.size(); ++i) {
      auto const value = lastValues[i];
      auto const view = std::views::counted(lastValues.begin() + i - PreambleSize, PreambleSize);
      auto const iterPair = AoC::findPair(view, [value](auto const a, auto const b) { return a + b == value; });
      if (iterPair.first == view.end()) {
        assert(iterPair.second == view.end());
        return value;
      }
    }
    throw std::runtime_error("Should be unreachable.");
  }();
  if (part == 1) {
    std::cout << firstViolation << '\n';
  } else {
    auto itA = lastValues.begin();
    auto itB = std::next(std::next(itA));
    auto sum = AoC::sum(std::ranges::subrange(itA, itB));
    while (true) {
      if (sum == firstViolation) {
        auto const [min, max] = std::minmax_element(itA, itB);
        std::cout << *min + *max << '\n';
        break;
      } else if (sum > firstViolation && std::distance(itA, itB) > 2) {
        sum -= *itA;
        ++itA;
      } else {
        assert(itB != lastValues.end());
        sum += *itB;
        ++itB;
      }
    }
  }
}
} // namespace AoC

#include "Main.inl"
