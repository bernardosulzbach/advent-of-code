#include "ArgumentParser.hpp"
#include "Types.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <ranges>
#include <unordered_map>

namespace AoC {
auto const NotSaidYet = std::numeric_limits<U32>::max();

void main(std::istream &stream, U32 const part) {
  std::vector<U32> initialValues;
  {
    U32 value;
    while (stream >> value) {
      initialValues.push_back(value);
      stream.ignore(1);
    }
  }
  assert(!initialValues.empty());
  auto const target = part == 1 ? 2020u : 30'000'000u;
  std::vector<U32> lastSaidMap(target, NotSaidYet);
  U32 lastSaidValue = initialValues[0];
  for (U32 i = 1; i < target; i++) {
    if (i < initialValues.size()) {
      lastSaidMap[lastSaidValue] = i; // Lines are 1-indexed.
      lastSaidValue = initialValues[i];
    } else {
      U32 delta = 0;
      if (lastSaidMap[lastSaidValue] != NotSaidYet) {
        delta = i - lastSaidMap[lastSaidValue];
      }
      lastSaidMap[lastSaidValue] = i;
      lastSaidValue = delta;
    }
  }
  std::cout << lastSaidValue << '\n';
}
} // namespace AoC

#include "Main.inl"
