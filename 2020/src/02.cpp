#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Interval.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace AoC {
void main(ArgumentParser const &argumentParser) {
  auto const part = argumentParser.getPart();
  std::ifstream stream(argumentParser.getPath());
  int validPasswords = 0;
  int a;
  while (stream >> a) {
    stream.ignore();
    int b;
    stream >> b;
    char character;
    stream >> character;
    stream.ignore(2);
    std::string password;
    stream >> password;
    auto const count = std::count(std::begin(password), std::end(password), character);
    if (part == 1 && AoC::Interval(a, b + 1).contains(count)) {
      validPasswords++;
    } else if (part == 2 && ((password[a - 1] == character) ^ (password[b - 1] == character)) != 0) {
      validPasswords++;
    }
  }
  std::cout << validPasswords << '\n';
}
} // namespace AoC

#include "Main.inl"