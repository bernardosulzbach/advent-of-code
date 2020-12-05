#pragma once

#include "Types.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_set>

class BugScan {
  constexpr static S32 Side = 5;

  U32 state = 0;

  [[nodiscard]] U32 get(U32 i, U32 j) const {
    return state >> (i * static_cast<U32>(Side) + j) & 1u;
  }

  [[nodiscard]] U32 countNeighbors(S32 i, S32 j) const {
    U32 count = 0;
    count += get(i - 1, j);
    count += get(i, j - 1);
    count += get(i, j + 1);
    count += get(i + 1, j);
    return count;
  }

public:
  [[nodiscard]] U32 get(S32 i, S32 j) const {
    if (i < 0 || i >= Side || j < 0 || j >= Side) {
      return 0;
    }
    return get(static_cast<U32>(i), static_cast<U32>(j));
  }

  void set(U32 i, U32 j, U32 x) {
    assert(x == 0 || x == 1);
    const U32 oneHot = 1u << (i * static_cast<U32>(Side) + j);
    if (x == 0) {
      state &= ~oneHot;
    }
    if (x == 1) {
      state |= oneHot;
    }
  }

  [[nodiscard]] BugScan evolve() const {
    BugScan evolution = *this;
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

  [[nodiscard]] U32 getBiodiversity() const {
    return state;
  }

  bool operator==(const BugScan &rhs) const {
    return state == rhs.state;
  }

  bool operator!=(const BugScan &rhs) const {
    return !(rhs == *this);
  }

  [[nodiscard]] std::string toString() const {
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
    std::stringstream stream(string);
    BugScan scan;
    for (U32 i = 0; i < Side; i++) {
      for (U32 j = 0; j < Side; j++) {
        char character = '\0';
        if (!(stream >> character)) {
          throw std::invalid_argument("Could not extract a character.");
        }
        if (character == '#') {
          scan.set(i, j, 1);
        } else if (character == '.') {
          scan.set(i, j, 0);
        } else {
          throw std::invalid_argument("Read an unexpected character.");
        }
      }
    }
    return scan;
  }

  [[nodiscard]] std::size_t hash() const {
    return state;
  }
};

template <> struct std::hash<BugScan> {
  std::size_t operator()(const BugScan &scan) const {
    return scan.hash();
  }
};
