#include "ArgumentParser.hpp"
#include "Casts.hpp"
#include "Text.hpp"
#include "Types.hpp"
#include "Verify.hpp"

#include <algorithm>
#include <cassert>
#include <charconv>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <string_view>
#include <vector>

namespace AoC {
void main(std::istream &stream, U32 const part) {
  std::string line;
  if (part == 1) {
    std::vector<std::uint16_t> stack(10007);
    std::iota(std::begin(stack), std::end(stack), decltype(stack)::value_type{});
    while (std::getline(stream, line)) {
      if (!line.empty()) {
        auto const afterStart = [&](std::string_view const &string) -> std::optional<int> {
          if (line.starts_with(string)) {
            int value;
            auto [_, ec] =
                std::from_chars(std::data(line) + std::size(string), std::data(line) + std::size(line), value);
            assert(ec == std::errc());
            return std::make_optional(value);
          } else {
            return std::nullopt;
          }
        };
        if (line.starts_with("deal into new stack")) {
          std::ranges::reverse(stack);
        } else if (auto const maybeCut = afterStart("cut ")) {
          auto cut = *maybeCut;
          assert(std::abs(cut) < std::size(stack));
          assert(cut != 0);
          if (cut < 0) {
            cut += static_cast<int>(std::size(stack));
          }
          assert(cut > 0);
          std::rotate(std::begin(stack), std::begin(stack) + cut, std::end(stack));
        } else if (auto const maybeIncrement = afterStart("deal with increment ")) {
          auto const increment = unsignedCast<Checked>(*maybeIncrement);
          std::size_t j = 0;
          auto newStack = stack;
          for (std::size_t i = 0; i < std::size(stack); ++i) {
            newStack[j] = stack[i];
            j = (j + increment) % std::size(stack);
          }
          stack = std::move(newStack);
        } else {
          assert(false);
        }
      }
    }
    std::cout << std::distance(std::begin(stack), std::find(std::begin(stack), std::end(stack), 2019)) << '\n';
  } else {
    assert(false);
  }
}
} // namespace AoC

#include "Main.inl"
