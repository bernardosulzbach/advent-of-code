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
  using Position = std::array<int, 2>;
  std::map<Position, int> flips;
  for (auto const &line : readLines(stream)) {
    Position p{};
    for (std::size_t i = 0; i < std::size(line); ++i) {
      if (line[i] == 'n' || line[i] == 's') {
        if (line[i] == 'n') {
          ++p[1];
        } else {
          --p[0];
          --p[1];
        }
        ++i;
        assert(line[i] == 'e' || line[i] == 'w');
        if (line[i] == 'e') {
          ++p[0];
        }
      } else {
        assert(line[i] == 'e' || line[i] == 'w');
        if (line[i] == 'e') {
          ++p[0];
        } else {
          --p[0];
        }
      }
    }
    ++flips[p];
  }
  std::set<Position> blacks;
  for (auto const &[p, flipCount] : flips) {
    if (flipCount % 2 == 1) {
      blacks.insert(p);
    }
  }
  if (part == 1) {
    std::cout << std::size(blacks) << '\n';
  } else {
    assert(part == 2);
    for (int day = 1; day <= 100; ++day) {
      {
        std::map<Position, int> adjacentBlacks;
        for (auto const &blackPosition : blacks) {
          auto const adjacentPositions = [](Position const p) noexcept {
            std::array<Position, 6> adjacent;
            std::size_t j = 0;
            auto const add = [&](int const dx, int const dy) noexcept {
              adjacent[j] = p;
              adjacent[j][0] += dx;
              adjacent[j][1] += dy;
              ++j;
            };
            add(0, 1);
            add(1, 1);
            add(-1, 0);
            add(1, 0);
            add(-1, -1);
            add(0, -1);
            return adjacent;
          };
          for (auto const p : adjacentPositions(blackPosition)) {
            ++adjacentBlacks[p];
          }
        }
        std::set<Position> newBlacks;
        for (auto const &[p, adjacentBlackCount] : adjacentBlacks) {
          auto const isBlack = blacks.contains(p);
          if ((isBlack && !(adjacentBlackCount == 0 || adjacentBlackCount > 2)) ||
              (!isBlack && adjacentBlackCount == 2)) {
            newBlacks.insert(p);
          }
        }
        blacks = std::move(newBlacks);
      }
      if (day < 10 || day % 10 == 0) {
        std::cout << "Day " << day << ": " << std::size(blacks) << "\n";
      }
    }
  }
}
} // namespace AoC

#include "Main.inl"
