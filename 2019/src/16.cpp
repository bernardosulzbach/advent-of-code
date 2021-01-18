#include "ArgumentParser.hpp"
#include "Body.hpp"
#include "Cycle.hpp"
#include "DirectedGraph.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Point.hpp"
#include "Screen.hpp"
#include "Text.hpp"
#include "Vector.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace AoC {
void fftPhase(std::vector<S32> const &input, std::vector<S32> &output) noexcept {
  const std::array<S32, 4> coefficients = {0, 1, 0, -1};
  for (std::size_t i = 0; i < input.size(); i++) {
    output[i] = 0;
    for (std::size_t j = 0; j < input.size(); j++) {
      const auto coefficient = coefficients[((j + 1) / (i + 1)) % coefficients.size()];
      output[i] += (coefficient * input[j]) % 10;
    }
    output[i] = std::abs(output[i] % 10);
  }
}

void main(ArgumentParser const &argumentParser) {
  std::vector<S32> sequence;
  {
    std::ifstream stream(argumentParser.getPath());
    char x;
    while (stream >> x) {
      sequence.push_back(x - '0');
    }
  }
  if (argumentParser.getPart() == 1) {
    std::vector<S32> output(sequence.size());
    for (U32 phase = 1; phase <= 100; phase++) {
      fftPhase(sequence, output);
      sequence = output;
    }
    for (std::size_t i = 0; i < 8; i++) {
      std::cout << sequence[i];
    }
    std::cout << '\n';
  } else {
    throw std::runtime_error("Not implemented.");
  }
}
} // namespace AoC

#include "Main.inl"
