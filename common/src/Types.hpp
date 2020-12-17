#pragma once

#include <cstdint>
#include <type_traits>

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

using S32 = int32_t;
using S64 = int64_t;

using F32 = float;
using F64 = double;

namespace AoC {
template <typename T> decltype(auto) toUnsigned(T const t) noexcept {
  static_assert(std::is_signed_v<T>);
  assert(0 <= t);
  return static_cast<std::make_unsigned_t<T>>(t);
}
} // namespace AoC