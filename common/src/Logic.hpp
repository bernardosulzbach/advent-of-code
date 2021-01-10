#pragma once

namespace AoC {
template <typename Ta, typename Tb> [[nodiscard]] constexpr bool isAnyOf(Ta const a, Tb const b) noexcept {
  return a == b;
}

template <typename Ta, typename Tb, typename... Ts>
[[nodiscard]] constexpr bool isAnyOf(Ta const a, Tb const b, Ts const... ts) noexcept {
  return a == b || isAnyOf(a, ts...);
}
} // namespace AoC