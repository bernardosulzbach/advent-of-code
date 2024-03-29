#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Interval.hpp"
#include "Ranges.hpp"
#include "Types.hpp"
#include "Vector.hpp"

#include <algorithm>
#include <cassert>

#include <iostream>
#include <ranges>
#include <vector>

namespace AoC {
inline constexpr std::size_t PreambleSize = 25;

inline constexpr auto OccupiedSeat = '#';
inline constexpr auto EmptySeat = 'L';
inline constexpr auto EmptySpace = '.';

inline constexpr auto Undefined = std::numeric_limits<std::size_t>::max();

using DistanceArray = std::array<std::size_t, DirectionVectors.size()>;

[[nodiscard]] constexpr Vector<S64, 2> sizesToVector(std::size_t const i, std::size_t const j) noexcept {
  return Vector<S64, 2>{static_cast<S64>(i), static_cast<S64>(j)};
}

void main(std::istream &stream, U32 const part) {
  auto lines = AoC::readVector<std::string>(stream);
  auto const iInterval = AoC::Interval<S64>(0, static_cast<S64>(lines.size()));
  auto const jInterval = AoC::Interval<S64>(0, static_cast<S64>(lines.front().size()));
  auto updatedLines = lines;
  std::vector<std::vector<DistanceArray>> distanceToNeighbors(lines.size(),
                                                              std::vector<DistanceArray>(lines.front().size()));
  auto const countOccupiedNeighbors = [part, &lines, iInterval, jInterval,
                                       &distanceToNeighbors](std::size_t const i, std::size_t const j) noexcept {
    assert(lines[i][j] != EmptySpace);
    std::size_t count = 0;
    if (part == 1) {
      for (auto const vector : DirectionVectors) {
        auto const n = sizesToVector(i, j) + vector;
        if (iInterval.contains(n[0]) && jInterval.contains(n[1])) {
          if (lines[unsignedCast<Unchecked>(n[0])][unsignedCast<Unchecked>(n[1])] == OccupiedSeat) {
            count++;
          }
        }
      }
    } else if (part == 2) {
      for (std::size_t k = 0; k < DirectionVectors.size(); k++) {
        if (distanceToNeighbors[i][j][k] == Undefined) {
          continue;
        }
        auto const n = sizesToVector(i, j) + DirectionVectors[k].scale(static_cast<S64>(distanceToNeighbors[i][j][k]));
        assert(iInterval.contains(n[0]) && jInterval.contains(n[1]));
        assert(lines[unsignedCast<Unchecked>(n[0])][unsignedCast<Unchecked>(n[1])] != EmptySpace);
        if (lines[unsignedCast<Unchecked>(n[0])][unsignedCast<Unchecked>(n[1])] == OccupiedSeat) {
          count++;
        }
      }
    }
    return count;
  };
  if (part == 2) {
    auto const findDistanceToNeighbor = [&lines, iInterval, jInterval](Vector<S64, 2> const p,
                                                                       Vector<S64, 2> const d) noexcept {
      auto n = p + d;
      std::size_t distance = 1;
      while (iInterval.contains(n[0]) && jInterval.contains(n[1]) &&
             lines[unsignedCast<Unchecked>(n[0])][unsignedCast<Unchecked>(n[1])] == EmptySpace) {
        n += d;
        distance++;
      }
      if (iInterval.contains(n[0]) && jInterval.contains(n[1])) {
        assert(lines[unsignedCast<Unchecked>(n[0])][unsignedCast<Unchecked>(n[1])] != EmptySpace);
        return distance;
      }
      return Undefined;
    };
    for (std::size_t i = 0; i < lines.size(); i++) {
      for (std::size_t j = 0; j < lines.front().size(); j++) {
        if (lines[i][j] != EmptySpace) {
          for (std::size_t k = 0; k < DirectionVectors.size(); k++) {
            distanceToNeighbors[i][j][k] = findDistanceToNeighbor(sizesToVector(i, j), DirectionVectors[k]);
          }
        }
      }
    }
  }
  std::size_t const EmptyingThreshold = part == 1 ? 4 : 5;
  for (auto changed = true; changed;) {
    changed = false;
    for (std::size_t i = 0; i < lines.size(); i++) {
      for (std::size_t j = 0; j < lines.front().size(); j++) {
        if (lines[i][j] == EmptySeat && countOccupiedNeighbors(i, j) == 0) {
          updatedLines[i][j] = OccupiedSeat;
          changed = true;
        } else if (lines[i][j] == OccupiedSeat && countOccupiedNeighbors(i, j) >= EmptyingThreshold) {
          updatedLines[i][j] = EmptySeat;
          changed = true;
        } else {
          updatedLines[i][j] = lines[i][j];
        }
      }
    }
    lines.swap(updatedLines);
  }
  std::cout << std::ranges::count(std::ranges::views::join(lines), OccupiedSeat) << '\n';
}
} // namespace AoC

#include "Main.inl"
