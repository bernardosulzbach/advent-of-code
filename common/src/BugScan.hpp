#pragma once

#include "Math.hpp"
#include "Types.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_set>

namespace AoC {
class BugScan {
  using CoordinateType = S32;
  using UnsignedCoordinateType = std::make_unsigned_t<CoordinateType>;

  static constexpr CoordinateType Side = 5;

  U32 state = 0;

  [[nodiscard]] constexpr UnsignedCoordinateType toIndex(UnsignedCoordinateType const i,
                                                         UnsignedCoordinateType const j) const noexcept {
    return i * unsignedCast<Checked>(Side) + j;
  }

  [[nodiscard]] U32 get(UnsignedCoordinateType const i, UnsignedCoordinateType const j) const noexcept {
    return state >> toIndex(i, j) & 1u;
  }

  [[nodiscard]] U32 countNeighbors(CoordinateType const i, CoordinateType const j) const noexcept {
    U32 count = 0;
    count += get(i - 1, j);
    count += get(i, j - 1);
    count += get(i, j + 1);
    count += get(i + 1, j);
    return count;
  }

public:
  [[nodiscard]] U32 get(CoordinateType const i, CoordinateType const j) const noexcept {
    if (i < 0 || i >= Side || j < 0 || j >= Side) {
      return 0;
    }
    return get(unsignedCast<Checked>(i), unsignedCast<Checked>(j));
  }

  void set(CoordinateType const i, CoordinateType const j, U32 const x) noexcept {
    assert(x == 0 || x == 1);
    auto const oneHot = 1u << toIndex(unsignedCast<Checked>(i), unsignedCast<Checked>(j));
    if (x == 0) {
      state &= ~oneHot;
    }
    if (x == 1) {
      state |= oneHot;
    }
  }

  [[nodiscard]] BugScan evolve() const noexcept {
    auto evolution = *this;
    for (S32 i = 0; i < Side; i++) {
      for (S32 j = 0; j < Side; j++) {
        const auto count = countNeighbors(i, j);
        if (get(i, j) == 1) {
          if (count != 1) {
            evolution.set(i, j, 0);
          }
        } else {
          if (count == 1 || count == 2) {
            evolution.set(i, j, 1);
          }
        }
      }
    }
    return evolution;
  }

  [[nodiscard]] U32 getBiodiversity() const noexcept {
    return state;
  }

  [[nodiscard]] constexpr auto operator<=>(BugScan const &rhs) const noexcept = default;

  [[nodiscard]] std::string toString() const noexcept {
    std::string string;
    for (S32 i = 0; i < BugScan::Side; i++) {
      for (S32 j = 0; j < BugScan::Side; j++) {
        string += (get(i, j) ? '#' : '.');
      }
      if (i + 1 < BugScan::Side) {
        string += '\n';
      }
    }
    return string;
  }

  static BugScan fromString(const std::string &string) {
    BugScan scan;
    std::stringstream stream(string);
    for (CoordinateType i = 0; i < Side; i++) {
      for (CoordinateType j = 0; j < Side; j++) {
        auto character = '\0';
        if (!(stream >> character)) {
          throw std::invalid_argument("Could not extract a character.");
        }
        if (character == '#') {
          scan.set(i, j, 1u);
        } else if (character == '.') {
          scan.set(i, j, 0u);
        } else {
          throw std::invalid_argument("Read an unexpected character.");
        }
      }
    }
    return scan;
  }

  [[nodiscard]] std::size_t hash() const noexcept {
    return state;
  }
};
} // namespace AoC

template <> struct std::hash<AoC::BugScan> {
  std::size_t operator()(const AoC::BugScan &scan) const {
    return scan.hash();
  }
};
