#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Math.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
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
      if (part == 1 && Interval(a, b).contains(count)) {
        validPasswords++;
      } else if (part == 2 && ((password[a - 1] == character) ^ (password[b - 1] == character)) != 0) {
        validPasswords++;
      }
    }
    std::cout << validPasswords << '\n';
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
