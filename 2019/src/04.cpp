#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Types.hpp"

#include <iostream>

namespace AoC {
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

void main(std::istream &stream, U32 const part) {
  auto const a = AoC::readValue<U32>(stream);
  stream.ignore();
  auto const b = AoC::readValue<U32>(stream);
  U32 valid = 0;
  for (U32 i = a; i <= b; i++) {
    if (isValid(i, part == 2)) {
      valid++;
    }
  }
  std::cout << valid << '\n';
}
} // namespace AoC

#include "Main.inl"
