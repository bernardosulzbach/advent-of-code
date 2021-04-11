#include "ArgumentParser.hpp"
#include "Casts.hpp"
#include "Containers.hpp"
#include "IO.hpp"
#include "Interval.hpp"
#include "Text.hpp"
#include "Types.hpp"

#include <algorithm>

#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace AoC {
template <typename T> [[nodiscard]] T wrappingDecrement(T const value, Interval<T> const interval) noexcept {
  if (value == interval.start) {
    return interval.finish;
  } else {
    return value - 1;
  }
}

void main(std::istream &stream, U32 const part) noexcept {
  auto cups = readValue<std::string>(stream);
  if (part == 1) {
    for (S32 move = 0; move < 100; ++move) {
      auto const taken = cups.substr(1, 3);
      cups.erase(std::begin(cups) + 1, std::begin(cups) + 4);
      auto insertionPoint = wrappingDecrement(cups[0], {'0', '9'});
      while (cups.find(insertionPoint) == std::string::npos) {
        insertionPoint = wrappingDecrement(insertionPoint, {'0', '9'});
      }
      cups.insert(cups.find(insertionPoint) + 1, taken);
      cups.push_back(cups.front());
      cups.erase(std::begin(cups));
    }
    auto const shiftAmount = signedCast<Unchecked>((cups.find('1') + 1) % cups.size());
    std::ranges::rotate(std::begin(cups), std::begin(cups) + shiftAmount, std::end(cups));
    std::cout << cups.substr(0, cups.size() - 1) << "\n";
  }
}
} // namespace AoC

#include "Main.inl"
