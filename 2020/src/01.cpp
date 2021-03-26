#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

#include <iostream>

namespace AoC {
template <std::size_t N, typename Range> void findCombination(Range const &input) {
  for (auto const combination : AoC::allCombinations<N>(input)) {
    if (AoC::sum(combination) == 2020) {
      std::cout << AoC::product(combination) << '\n';
      break;
    }
  }
}

void main(std::istream &stream, U32 const part) {
  auto const input = AoC::readVector<S64>(stream);
  if (part == 1) {
    findCombination<2>(input);
  } else {
    findCombination<3>(input);
  }
}
} // namespace AoC

#include "Main.inl"
