#include "ArgumentParser.hpp"

#include <iostream>

namespace AoC {
int fuelRequired(int weight) {
  return weight / 3 - 2;
}

void main(std::istream &stream, U32 const part) {
  int sum = 0;
  int value;
  while (stream >> value) {
    if (part == 1) {
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
