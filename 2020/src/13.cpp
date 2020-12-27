#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Math.hpp"
#include "Ranges.hpp"
#include "Text.hpp"
#include "Types.hpp"

#include <fstream>
#include <iostream>
#include <vector>

using Integer = S64;

struct BusScore {
  Integer id;
  Integer wait;

  static constexpr BusScore null() noexcept {
    return {0, std::numeric_limits<decltype(wait)>::max()};
  }

  [[nodiscard]] bool operator<(BusScore const &rhs) const noexcept {
    return wait > rhs.wait;
  }
};

[[nodiscard]] BusScore calculateBusScore(Integer const earliestDeparture, Integer const id) noexcept {
  return BusScore{id, (id - earliestDeparture % id) % id};
}

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
    auto const earliestDeparture = AoC::readValue<Integer>(stream);
    auto const idLine = AoC::readValue<std::string>(stream);
    if (part == 1) {
      auto bestScore = BusScore::null();
      for (auto const &idString : splitAt(idLine, ",")) {
        if (idString == "x") {
          continue;
        }
        auto const id = AoC::parseValue<Integer>(idString);
        bestScore = std::max(bestScore, calculateBusScore(earliestDeparture, id));
      }
      std::cout << bestScore.id * bestScore.wait << '\n';
    } else {
      std::vector<Integer> n;
      std::vector<Integer> a;
      Integer remainder = 0;
      for (auto const &idString : splitAt(idLine, ",")) {
        if (idString != "x") {
          auto const id = AoC::parseValue<Integer>(idString);
          n.push_back(id);
          a.push_back(AoC::euclideanModulo(id - remainder, id));
        }
        remainder++;
      }
      for (std::size_t i = 0; i < n.size(); i++) {
        for (std::size_t j = i + 1; j < n.size(); j++) {
          n[j] /= greatestCommonDivisor(n[i], n[j]);
        }
      }
      std::cout << AoC::solveCongruenceSystem(n, a) << '\n';
    }
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
