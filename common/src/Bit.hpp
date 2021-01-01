#include <cassert>
#include <climits>
#include <type_traits>

namespace AoC {
template <typename T>
[[nodiscard]] constexpr T setBit(T const value, std::size_t const index, bool const bitValue) noexcept {
  static_assert(std::is_unsigned_v<T>);
  assert(index < sizeof(T) * CHAR_BIT);
  if (bitValue) {
    return value | (1ULL << index);
  } else {
    return value & ~(1ULL << index);
  }
}

template <typename T> [[nodiscard]] constexpr bool getBit(T const value, std::size_t const index) noexcept {
  static_assert(std::is_unsigned_v<T>);
  assert(index < sizeof(T) * CHAR_BIT);
  return (value >> index) & 1u;
}
} // namespace AoC