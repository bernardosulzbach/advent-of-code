#include "ArgumentParser.hpp"

#include <fstream>
#include <iostream>

namespace AoC {
int fuelRequired(int weight) {
  return weight / 3 - 2;
}

void main(ArgumentParser const &argumentParser) {
  int sum = 0;
  std::ifstream stream(argumentParser.getPath());
  int value;
  while (stream >> value) {
    if (argumentParser.getPart() == 1) {
      sum += fuelRequired(value);
    } else {
      while (fuelRequired(value) > 0) {
        value = fuelRequired(value);
        sum += value;
      }
    }
  }
  std::cout << sum << '\n';
}
} // namespace AoC

#include "Main.inl"
