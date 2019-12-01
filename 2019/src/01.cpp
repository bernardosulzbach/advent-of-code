#include <fstream>
#include <iostream>

int fuelRequired(int weight) { return weight / 3 - 2; }

void partOne() {
  std::ifstream stream("../input/01.txt");
  int sum = 0;
  int value;
  while (stream >> value) {
    sum += fuelRequired(value);
  }
  std::cout << sum << '\n';
}

void partTwo() {
  std::ifstream stream("../input/01.txt");
  int sum = 0;
  int value;
  while (stream >> value) {
    while (fuelRequired(value) > 0) {
      value = fuelRequired(value);
      sum += value;
    }
  }
  std::cout << sum << '\n';
}

int main() {
  partOne();
  partTwo();
  return 0;
}