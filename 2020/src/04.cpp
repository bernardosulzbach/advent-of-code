#include "ArgumentParser.hpp"
#include "Direction.hpp"
#include "IO.hpp"
#include "Point.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
    std::unordered_map<std::string, std::string> passport;
    std::string line;
    int validPassports = 0;
    auto const verifyPassport = [&passport, &validPassports, &part]() {
      auto const validatePassportFields = [&passport]() {
        std::array<char const *, 7> ExpectedFields = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
        return std::ranges::all_of(ExpectedFields, [&passport](auto const &field) { return passport.contains(field); });
      };
      auto const validatePassportValues = [&passport]() {
        auto const isNDigits = [](std::string const &string, std::size_t const n) {
          if (string.size() != n) {
            return false;
          }
          return std::ranges::all_of(string, [](auto const character) { return std::isdigit(character); });
        };
        auto const isNDigitsInRange = [&isNDigits](std::string const &string, std::size_t const n,
                                                   std::string const &low, std::string const &high) {
          assert(low.size() == n);
          assert(high.size() == n);
          return isNDigits(string, n) && low <= string && string <= high;
        };
        auto const isValidHexadecimalColor = [](std::string const &string) {
          if (string.size() != 7) {
            return false;
          }
          auto const isHexChar = [](auto const c) { return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f'); };
          return string[0] == '#' && std::ranges::all_of(std::ranges::drop_view(string, 1), isHexChar);
        };
        if (!isNDigitsInRange(passport["byr"], 4, "1920", "2002")) {
          return false;
        }
        if (!isNDigitsInRange(passport["iyr"], 4, "2010", "2020")) {
          return false;
        }
        if (!isNDigitsInRange(passport["eyr"], 4, "2020", "2030")) {
          return false;
        }
        auto const isValidHeight = [&isNDigitsInRange](std::string const &string) {
          if (string.ends_with("cm") && isNDigitsInRange(string.substr(0, string.size() - 2), 3, "150", "193")) {
            return true;
          }
          return string.ends_with("in") && isNDigitsInRange(string.substr(0, string.size() - 2), 2, "59", "76");
        };
        if (!isValidHeight(passport["hgt"])) {
          return false;
        }
        if (!isValidHexadecimalColor(passport["hcl"])) {
          return false;
        }
        auto const ValidEyeColors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        if (std::ranges::find(ValidEyeColors, passport["ecl"]) == std::ranges::end(ValidEyeColors)) {
          return false;
        }
        return isNDigits(passport["pid"], 9);
      };
      if (validatePassportFields() && (part == 1 || (part == 2 && validatePassportValues()))) {
        validPassports++;
      }
      passport.clear();
    };
    while (std::getline(stream, line)) {
      if (line.empty()) {
        verifyPassport();
        continue;
      }
      std::stringstream lineStream(line);
      std::string entry;
      while (lineStream >> entry) {
        auto const separatorIterator = entry.find(':');
        assert(separatorIterator != std::string::npos);
        passport[entry.substr(0, separatorIterator)] = entry.substr(separatorIterator + 1);
      }
    }
    verifyPassport();
    std::cout << validPassports << '\n';
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
