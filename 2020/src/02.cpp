#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Interval.hpp"
#include "Types.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace AoC {
void main(ArgumentParser const &argumentParser) {
  auto const part = argumentParser.getPart();
  std::ifstream stream(argumentParser.getPath());
  U32 validPasswords = 0;
  U32 a;
  while (stream >> a) {
    stream.ignore();
    U32 b;
    stream >> b;
    char character;
    stream >> character;
    stream.ignore(2);
    std::string password;
    stream >> password;
    auto const count = std::count(std::begin(password), std::end(password), character);
    if (part == 1 && AoC::Interval(a, b + 1).contains(static_cast<decltype(a)>(count))) {
      validPasswords++;
    } else if (part == 2 && ((password[a - 1] == character) ^ (password[b - 1] == character)) != 0) {
      validPasswords++;
    }
  }
  std::cout << validPasswords << '\n';
}
} // namespace AoC

#include "Main.inl"