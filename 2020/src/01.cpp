#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

#include <fstream>
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

void main(ArgumentParser const &argumentParser) {
  std::ifstream stream(argumentParser.getPath());
  auto const input = AoC::readVector<S64>(stream);
  if (argumentParser.getPart() == 1) {
    findCombination<2>(input);
  } else {
    findCombination<3>(input);
  }
}
} // namespace AoC

#include "Main.inl"