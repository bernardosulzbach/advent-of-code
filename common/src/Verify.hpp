#pragma once

#include <cassert>

namespace AoC {
template <typename T> constexpr void verify(T &&t) noexcept {
  assert(std::forward<T>(t));
  static_cast<void>(t);
}
} // namespace AoC