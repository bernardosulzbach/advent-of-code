#include "ArgumentParser.hpp"

#include <fstream>
#include <iostream>

int fuelRequired(int weight) {
  return weight / 3 - 2;
}

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto part = argumentParser.getAdditionalArgument(0);
  int sum = 0;
  std::ifstream stream(argumentParser.getPath());
  int value;
  while (stream >> value) {
    if (part == 1) {
      sum += fuelRequired(value);
    } else if (part == 2) {
      while (fuelRequired(value) > 0) {
        value = fuelRequired(value);
        sum += value;
      }
    } else {
      throw std::runtime_error("Part should be 1 or 2.");
    }
  }
  std::cout << sum << '\n';
  return 0;
}
