#include "ArgumentParser.hpp"
#include "Casts.hpp"
#include "Intcode.hpp"
#include "Point.hpp"
#include "Text.hpp"
#include "Verify.hpp"

#include <cassert>
#include <iostream>
#include <optional>

namespace AoC {
void main(std::istream &stream, U32 const part) {
  const auto originalIntcode = Intcode(readMemory(stream));
  const auto test = [&](Intcode::ValueType const x, Intcode::ValueType const y) {
    auto intcode = originalIntcode;
    verify(intcode.run() == IntcodeState::Blocked);
    intcode.addInput(x);
    intcode.addInput(y);
    verify(intcode.run() == IntcodeState::Halted);
    assert(intcode.hasOutput());
    return intcode.getOutput();
  };
  if (part == 1) {
    const Intcode::ValueType size = 50;
    S32 count = 0;
    for (Intcode::ValueType i = 0; i < size; i++) {
      for (Intcode::ValueType j = 0; j < size; j++) {
        if (test(j, i) == 1) {
          count++;
        }
      }
    }
    std::cout << count << '\n';
  } else {
    // Empirically determined.
    const Intcode::ValueType size = 2000;
    using UnsignedType = std::make_unsigned_t<std::decay_t<decltype(size)>>;
    std::vector<std::vector<bool>> grid(size, std::vector<bool>(size));
    for (Intcode::ValueType i = 0; i < size; i++) {
      for (Intcode::ValueType j = 0; j < size; j++) {
        if (test(j, i) == 1) {
          grid[unsignedCast<Unchecked>(i)][unsignedCast<Unchecked>(j)] = true;
        }
      }
    }
    std::vector<std::vector<UnsignedType>> lengthRight(size + 1, std::vector<UnsignedType>(size + 1));
    std::vector<std::vector<UnsignedType>> lengthDown(size + 1, std::vector<UnsignedType>(size + 1));
    for (Intcode::ValueType i = size - 1; i >= 0; i--) {
      for (Intcode::ValueType j = size - 1; j >= 0; j--) {
        auto const uI = unsignedCast<Unchecked>(i);
        auto const uJ = unsignedCast<Unchecked>(j);
        if (grid[uI][uJ]) {
          lengthRight[uI][uJ] = lengthRight[uI][uJ + 1u] + 1;
          lengthDown[uI][uJ] = lengthDown[uI + 1u][uJ] + 1;
        } else {
          lengthRight[uI][uJ] = 0;
          lengthDown[uI][uJ] = 0;
        }
      }
    }
    std::optional<Point<Intcode::ValueType, 2>> closestPoint;
    const auto getPointDistanceSquare = [](std::optional<Point<Intcode::ValueType, 2>> const &optionalPoint) noexcept {
      if (!optionalPoint) {
        return std::numeric_limits<Intcode::ValueType>::max();
      }
      return optionalPoint->toOrigin().getL2NormSquare();
    };
    for (Intcode::ValueType i = 0; i < size; i++) {
      for (Intcode::ValueType j = 0; j < size; j++) {
        auto const uI = unsignedCast<Unchecked>(i);
        auto const uJ = unsignedCast<Unchecked>(j);
        if (lengthRight[uI][uJ] >= 100 && lengthDown[uI][uJ] >= 100) {
          const auto point = Point<Intcode::ValueType, 2>(i, j);
          if (getPointDistanceSquare(point) < getPointDistanceSquare(closestPoint)) {
            closestPoint = point;
          }
        }
      }
    }
    std::cout << 10000 * closestPoint->getX() + closestPoint->getY() << '\n';
    assert(closestPoint.has_value());
  }
}
} // namespace AoC

#include "Main.inl"
