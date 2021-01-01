#include <string_view>
#include <vector>

#include "Types.hpp"

namespace AoC {
using BitMaskType = U64;

class BitMask {
  BitMaskType orVector{};
  BitMaskType andVector = ~BitMaskType{};
  BitMaskType floatingVector{};

  void applyFrom(std::vector<BitMaskType> &outputVector, BitMaskType currentValue, std::size_t bitIndex) const noexcept;

public:
  /**
   * Constructs a bit mask from a bitmask string.
   *
   * A bitmask string is a string of '0', '1', '-', and '*'.
   *
   * '0' and '1' specify the value the bit should be set to.
   * '-' (don't care) specifies that the bit should be left unchanged.
   * '*' (floating) specifies that the bit should be set to 0 and to 1.
   *
   * If the override string is not long enough, it is left padded with '-'.
   */
  static BitMask bitMaskFromString(std::string_view overrideString) noexcept;

  [[nodiscard]] std::vector<BitMaskType> apply(BitMaskType input) const noexcept;
};
} // namespace AoC