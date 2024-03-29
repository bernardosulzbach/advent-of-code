#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Point.hpp"

#include <algorithm>
#include <cassert>

#include <iostream>
#include <string>
#include <unordered_map>

namespace AoC {
[[nodiscard]] int binaryPartition(int a, int b, std::string const &string, char const upperChar) noexcept {
  auto skip = (b - a) / 2;
  for (auto const c : string) {
    if (c == upperChar) {
      a += skip;
    } else {
      b -= skip;
    }
    skip /= 2;
  }
  assert(a + 1 == b);
  return a;
}

[[nodiscard]] auto findSeatPoint(std::string const &string) noexcept {
  auto const y = binaryPartition(0, 128, string.substr(0, 7), 'B');
  auto const x = binaryPartition(0, 8, string.substr(7), 'R');
  return Point<S32, 2>(x, y);
}

[[nodiscard]] auto seatId(Point<S32, 2> const point) noexcept {
  return point.getY() * 8 + point.getX();
}

void main(std::istream &stream, U32 const part) {
  assert((findSeatPoint("FBFBBFFRLR") == Point<S32, 2>(5, 44)));
  assert(seatId(findSeatPoint("FBFBBFFRLR")) == 357);
  std::vector<int> seatIds;
  {
    std::string string;
    while (stream >> string) {
      seatIds.push_back(seatId(findSeatPoint(string)));
    }
  }
  if (part == 1) {
    std::cout << *std::ranges::max_element(seatIds) << '\n';
  } else {
    std::ranges::sort(seatIds);
    auto const it = std::ranges::adjacent_find(seatIds, [](auto const a, auto const b) { return a + 2 == b; });
    assert(it != std::ranges::end(seatIds));
    std::cout << *it + 1 << '\n';
  }
}
} // namespace AoC

#include "Main.inl"
