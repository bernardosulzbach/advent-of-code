#include "ArgumentParser.hpp"
#include "Casts.hpp"
#include "Containers.hpp"
#include "IO.hpp"
#include "Text.hpp"
#include "Types.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace AoC {
void main(std::istream &stream, U32 const part) noexcept {
  static U64 constexpr M = 20201227;
  auto constexpr findLoopSize = [](auto const key) noexcept {
    static U64 constexpr subjectNumber = 7;
    U32 loopCount = 0;
    for (U64 value = 1; value != key; ++loopCount) {
      value = (value * subjectNumber) % M;
    }
    return loopCount;
  };
  U64 pC, pD;
  stream >> pC >> pD;
  if (part == 1) {
    auto const lC = findLoopSize(pC);
    auto const lD = findLoopSize(pD);
    auto const transform = [](auto const subjectNumber, auto const loopSize) {
      U64 value = 1;
      for (U32 i = 0; i < loopSize; ++i) {
        value = (value * subjectNumber) % M;
      }
      return value;
    };
    auto const e = transform(pC, lD);
    assert(e == transform(pD, lC));
    std::cout << lC << "\n" << lD << "\n" << e << "\n";
  }
}
} // namespace AoC

#include "Main.inl"
