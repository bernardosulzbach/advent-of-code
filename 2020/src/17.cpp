#include "ArgumentParser.hpp"
#include "BoundingBox.hpp"
#include "IO.hpp"
#include "Interval.hpp"
#include "Point.hpp"
#include "Text.hpp"
#include "Types.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <unordered_set>

namespace AoC {
static constexpr auto ActiveSymbol = '#';

template <Size Dimensions> void solve(std::istream &stream) {
  using CoordinateType = S16;
  using PointType = Point<CoordinateType, Dimensions>;

  std::unordered_set<PointType> active;
  {
    CoordinateType lineNumber = 0;
    for (auto const &line : AoC::readVector<std::string>(stream)) {
      for (Size i = 0; i < line.size(); i++) {
        if (line[i] == ActiveSymbol) {
          if constexpr (Dimensions == 3) {
            active.insert(PointType{static_cast<CoordinateType>(i), lineNumber, 0});
          } else {
            active.insert(PointType{static_cast<CoordinateType>(i), lineNumber, 0, 0});
          }
        }
      }
      lineNumber++;
    }
  }
  auto const step = [&active]() {
    std::unordered_map<PointType, U32> activeNeighbors;
    for (auto const activePoint : active) {
      for (auto const neighbor : activePoint.getNeighbors()) {
        activeNeighbors[neighbor]++;
      }
    }
    std::vector<PointType> becomingActive;
    for (auto const [point, count] : activeNeighbors) {
      if (count == 3 || (active.contains(point) && count == 2)) {
        becomingActive.push_back(point);
      }
    }
    active.clear();
    active.insert(std::begin(becomingActive), std::end(becomingActive));
  };
  for (U32 i = 0; i < 6; i++) {
    step();
  }
  std::cout << active.size() << '\n';
}

void main(ArgumentParser const &argumentParser) {
  auto stream = std::ifstream(argumentParser.getPath());
  if (argumentParser.getPart() == 1) {
    solve<3>(stream);
  } else {
    solve<4>(stream);
  }
}
} // namespace AoC

#include "Main.inl"
