#include "BitMask.hpp"

#include "Bit.hpp"
#include "Math.hpp"

#include <cassert>
#include <climits>
#include <string_view>
#include <type_traits>

namespace AoC {
static_assert(std::is_unsigned_v<BitMaskType>);

BitMask BitMask::bitMaskFromString(std::string_view const overrideString) noexcept {
  assert(!overrideString.empty());
  assert(overrideString.size() <= sizeof(BitMaskType) * CHAR_BIT);
  std::size_t bitIndex = 0;
  BitMask bitMask;
  for (auto it = std::rbegin(overrideString); it != std::rend(overrideString); ++it, bitIndex++) {
    if (*it == '0') {
      bitMask.andVector = setBit(bitMask.andVector, bitIndex, false);
    } else if (*it == '1') {
      bitMask.orVector = setBit(bitMask.orVector, bitIndex, true);
    } else if (*it == '*') {
      bitMask.floatingVector = setBit(bitMask.floatingVector, bitIndex, true);
    }
  }
  return bitMask;
}

std::vector<BitMaskType> BitMask::apply(BitMaskType const input) const noexcept {
  assert(std::popcount(floatingVector) <= 10); // Arbitrary hardcoded limit to prevent OOM.
  std::vector<BitMaskType> outputVector;
  applyFrom(outputVector, input, 0);
  assert(isPowerOfTwo(outputVector.size()));
  return outputVector;
}

void BitMask::applyFrom(std::vector<BitMaskType> &outputVector, BitMaskType const currentValue,
                        std::size_t const bitIndex) const noexcept {
  if (bitIndex == sizeof(BitMaskType) * CHAR_BIT) {
    outputVector.push_back(currentValue);
    return;
  }
  auto const isFloating = getBit(floatingVector, bitIndex);
  auto const output = getBit(andVector, bitIndex) && (getBit(orVector, bitIndex) || getBit(currentValue, bitIndex));
  if (isFloating || output) {
    applyFrom(outputVector, setBit(currentValue, bitIndex, true), bitIndex + 1);
  }
  if (isFloating || !output) {
    applyFrom(outputVector, setBit(currentValue, bitIndex, false), bitIndex + 1);
  }
}
} // namespace AoC