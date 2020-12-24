#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Math.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

inline constexpr std::size_t PreambleSize = 25;

inline constexpr auto OccupiedSeat = '#';
inline constexpr auto EmptySeat = 'L';
inline constexpr auto EmptySpace = '.';

inline constexpr auto Undefined = std::numeric_limits<std::size_t>::max();

struct Vector {
  S64 x;
  S64 y;
};

inline constexpr std::array<Vector, 8> DirectionVectors = {
    {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}}};

using DistanceArray = std::array<std::size_t, DirectionVectors.size()>;

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
    auto lines = AoC::readVector<std::string>(stream);
    auto updatedLines = lines;
    std::vector<std::vector<DistanceArray>> distanceToNeighbors(lines.size(),
                                                                std::vector<DistanceArray>(lines.front().size()));
    auto const countOccupiedNeighbors = [part, &lines, &distanceToNeighbors](std::size_t const i,
                                                                             std::size_t const j) noexcept {
      assert(lines[i][j] != EmptySpace);
      auto const iInterval = Interval<S64>(0, static_cast<S64>(lines.size()) - 1);
      auto const jInterval = Interval<S64>(0, static_cast<S64>(lines.front().size()) - 1);
      std::size_t count = 0;
      if (part == 1) {
        for (auto const vector : DirectionVectors) {
          auto const ni = static_cast<S64>(i) + vector.y;
          auto const nj = static_cast<S64>(j) + vector.x;
          if (iInterval.contains(ni) && jInterval.contains(nj) && lines[ni][nj] == OccupiedSeat) {
            count++;
          }
        }
      } else if (part == 2) {
        for (std::size_t k = 0; k < DirectionVectors.size(); k++) {
          if (distanceToNeighbors[i][j][k] == Undefined) {
            continue;
          }
          auto const ni = static_cast<S64>(i) + distanceToNeighbors[i][j][k] * DirectionVectors[k].y;
          auto const nj = static_cast<S64>(j) + distanceToNeighbors[i][j][k] * DirectionVectors[k].x;
          assert(iInterval.contains(ni) && jInterval.contains(nj));
          assert(lines[ni][nj] != EmptySpace);
          if (lines[ni][nj] == OccupiedSeat) {
            count++;
          }
        }
      }
      return count;
    };
    if (part == 2) {
      auto const findDistanceToNeighbor = [&lines](std::size_t const i, std::size_t const j, Vector const d) noexcept {
        auto const iInterval = Interval<S64>(0, static_cast<S64>(lines.size()) - 1);
        auto const jInterval = Interval<S64>(0, static_cast<S64>(lines.front().size()) - 1);
        auto ni = static_cast<S64>(i);
        auto nj = static_cast<S64>(j);
        ni += d.y;
        nj += d.x;
        std::size_t distance = 1;
        while (iInterval.contains(ni) && jInterval.contains(nj) && lines[ni][nj] == EmptySpace) {
          ni += d.y;
          nj += d.x;
          distance++;
        }
        if (iInterval.contains(ni) && jInterval.contains(nj)) {
          assert(lines[ni][nj] != EmptySpace);
          return distance;
        }
        return Undefined;
      };
      for (std::size_t i = 0; i < lines.size(); i++) {
        for (std::size_t j = 0; j < lines.front().size(); j++) {
          if (lines[i][j] != EmptySpace) {
            for (std::size_t k = 0; k < DirectionVectors.size(); k++) {
              distanceToNeighbors[i][j][k] = findDistanceToNeighbor(i, j, DirectionVectors[k]);
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
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
