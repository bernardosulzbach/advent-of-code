#include "ArgumentParser.hpp"
#include "Intcode.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

constexpr int AmplifierCount = 5;

void solvePartOne(const std::vector<Intcode::ValueType> &memory) {
  std::optional<std::array<int, AmplifierCount>> bestPhaseConfiguration;
  std::optional<int> bestOutputLevel;
  std::array<int, AmplifierCount> phaseConfiguration{0, 1, 2, 3, 4};
  do {
    std::vector<Intcode> amplifiers;
    for (int i = 0; i < 5; i++) {
      amplifiers.emplace_back(memory);
      amplifiers[i].addInput(phaseConfiguration[i]);
    }
    for (int i = 0; i < AmplifierCount; i++) {
      if (i == 0) {
        amplifiers[i].addInput(0);
      } else {
        amplifiers[i].addInput(amplifiers[i - 1].getOutput());
      }
      if (amplifiers[i].run() != IntcodeState::Halted) {
        throw std::runtime_error("Should have halted.");
      }
    }
    const auto outputLevel = amplifiers[AmplifierCount - 1].getOutput();
    if (!bestOutputLevel || outputLevel > bestOutputLevel.value()) {
      bestPhaseConfiguration = phaseConfiguration;
      bestOutputLevel = outputLevel;
    }
  } while (std::next_permutation(std::begin(phaseConfiguration), std::end(phaseConfiguration)));
  std::cout << bestOutputLevel.value() << '\n';
}

void solvePartTwo(const std::vector<Intcode::ValueType> &memory) {
  std::optional<std::array<int, AmplifierCount>> bestPhaseConfiguration;
  std::optional<int> bestOutputLevel;
  std::array<int, AmplifierCount> phaseConfiguration{5, 6, 7, 8, 9};
  do {
    std::vector<Intcode> amplifiers;
    for (int i = 0; i < 5; i++) {
      amplifiers.emplace_back(memory);
      amplifiers[i].addInput(phaseConfiguration[i]);
    }
    amplifiers[0].addInput(0);
    auto running = true;
    auto firstRun = true;
    while (running) {
      for (int i = 0; i < AmplifierCount; i++) {
        if (!firstRun) {
          amplifiers[i].addInput(amplifiers[(i + AmplifierCount - 1) % AmplifierCount].getOutput());
        }
        firstRun = false;
        if (amplifiers[i].run() == IntcodeState::Halted) {
          running = false;
        }
      }
    }
    const auto outputLevel = amplifiers[AmplifierCount - 1].getOutput();
    if (!bestOutputLevel || outputLevel > bestOutputLevel.value()) {
      bestPhaseConfiguration = phaseConfiguration;
      bestOutputLevel = outputLevel;
    }
  } while (std::next_permutation(std::begin(phaseConfiguration), std::end(phaseConfiguration)));
  std::cout << bestOutputLevel.value() << '\n';
}

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  std::ifstream stream(path);
  const auto memory = readMemory(path);
  int part = argumentParser.getAdditionalArgument(0);
  if (part == 1) {
    solvePartOne(memory);
  } else if (part == 2) {
    solvePartTwo(memory);
  } else {
    throw std::invalid_argument("Part should be 1 or 2.");
  }
  return 0;
}
