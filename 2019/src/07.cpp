#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <iostream>
#include <numeric>
#include <optional>

namespace AoC {
using ValueType = Intcode::ValueType;
static constexpr Size AmplifierCount = 5;

void run(std::vector<Intcode> &amplifiers, U32 const part) {
  if (part == 1) {
    for (Size i = 0; i < AmplifierCount; i++) {
      if (i == 0) {
        amplifiers[i].addInput(0);
      } else {
        amplifiers[i].addInput(amplifiers[i - 1].getOutput());
      }
      if (amplifiers[i].run() != IntcodeState::Halted) {
        throw std::runtime_error("Should have halted.");
      }
    }
  } else {
    amplifiers[0].addInput(0);
    auto running = true;
    auto firstRun = true;
    while (running) {
      for (Size i = 0; i < AmplifierCount; i++) {
        if (!firstRun) {
          amplifiers[i].addInput(amplifiers[(i + AmplifierCount - 1) % AmplifierCount].getOutput());
        }
        firstRun = false;
        if (amplifiers[i].run() == IntcodeState::Halted) {
          running = false;
        }
      }
    }
  }
}

void solve(std::vector<Intcode::ValueType> const &memory, U32 const part) {
  std::optional<std::array<ValueType, AmplifierCount>> bestPhaseConfiguration;
  std::optional<ValueType> bestOutputLevel;
  std::array<ValueType, AmplifierCount> phaseConfiguration;
  std::iota(std::begin(phaseConfiguration), std::end(phaseConfiguration), part == 1 ? 0 : 5);
  do {
    std::vector<Intcode> amplifiers;
    for (Size i = 0; i < AmplifierCount; i++) {
      amplifiers.emplace_back(memory);
      amplifiers[i].addInput(phaseConfiguration[i]);
    }
    run(amplifiers, part);
    const auto outputLevel = amplifiers[AmplifierCount - 1].getOutput();
    if (!bestOutputLevel || outputLevel > bestOutputLevel.value()) {
      bestPhaseConfiguration = phaseConfiguration;
      bestOutputLevel = outputLevel;
    }
  } while (std::next_permutation(std::begin(phaseConfiguration), std::end(phaseConfiguration)));
  std::cout << bestOutputLevel.value() << '\n';
}

void main(std::istream &stream, U32 const part) {
  solve(readMemory(stream), part);
}
} // namespace AoC

#include "Main.inl"
