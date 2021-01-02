#include "ArgumentParser.hpp"
#include "Types.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_map>

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
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
    std::unordered_map<U32, U32> lastSaidMap;
    U32 lastSaidValue = initialValues[0];
    for (U32 i = 1; i < target; i++) {
      if (i < initialValues.size()) {
        lastSaidMap[lastSaidValue] = i; // Lines are 1-indexed.
        lastSaidValue = initialValues[i];
      } else {
        U32 delta = 0;
        auto const it = lastSaidMap.find(lastSaidValue);
        if (it != lastSaidMap.end()) {
          delta = i - it->second;
        }
        lastSaidMap[lastSaidValue] = i;
        lastSaidValue = delta;
      }
    }
    std::cout << lastSaidValue << '\n';
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
