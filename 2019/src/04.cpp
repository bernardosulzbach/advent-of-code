#include "ArgumentParser.hpp"
#include "Types.hpp"

#include <fstream>
#include <iostream>

bool isValid(U32 password, bool exactlyTwoEqual) {
  U32 digit = password % 10;
  password /= 10;
  U32 repetitionLength = 1;
  auto repetitionCriterion = false;
  auto neverDecreases = true;
  const auto evaluateRepetitionCriterion = [exactlyTwoEqual, &repetitionLength, &repetitionCriterion]() {
    if (exactlyTwoEqual) {
      if (repetitionLength == 2) {
        repetitionCriterion = true;
      }
    } else {
      if (repetitionLength >= 2) {
        repetitionCriterion = true;
      }
    }
  };
  for (U32 i = 0; i < 5; i++) {
    U32 nextDigit = digit;
    digit = password % 10;
    password /= 10;
    if (digit == nextDigit) {
      repetitionLength++;
    } else {
      evaluateRepetitionCriterion();
      repetitionLength = 1;
    }
    if (digit > nextDigit) {
      neverDecreases = false;
    }
  }
  evaluateRepetitionCriterion();
  return repetitionCriterion && neverDecreases;
}

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  std::ifstream stream(path);
  U32 a;
  stream >> a;
  stream.ignore();
  U32 b;
  stream >> b;
  U32 validInPartOne = 0;
  U32 validInPartTwo = 0;
  for (U32 i = a; i <= b; i++) {
    if (isValid(i, false)) {
      validInPartOne++;
    }
    if (isValid(i, true)) {
      validInPartTwo++;
    }
  }
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part == 1) {
    std::cout << validInPartOne << '\n';
  } else if (part == 2) {
    std::cout << validInPartTwo << '\n';
  } else {
    throw std::runtime_error("Part should be 1 or 2.");
  }
  return 0;
}
