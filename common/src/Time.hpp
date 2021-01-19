#pragma once

#include "Math.hpp"
#include "Types.hpp"

#include <chrono>
#include <ostream>
#include <type_traits>

namespace AoC {
class [[nodiscard]] Duration {
public:
  [[nodiscard]] Duration() noexcept = default;

  [[nodiscard]] Duration(U64 const durationNanoseconds) noexcept : nanoseconds(durationNanoseconds) {
  }

  friend std::ostream &operator<<(std::ostream &stream, Duration const &duration) noexcept {
    struct Unit {
      U64 nanoseconds;
      char const *symbol;
    };
    constexpr std::array<Unit, 4> units{{{1'000'000'000UL, "s"}, {1'000'000UL, "ms"}, {1'000UL, "μs"}, {1UL, "ns"}}};
    Size i = 0;
    while (i + 1 < units.size() && duration.nanoseconds < units[i].nanoseconds) {
      i++;
    }
    return (stream << duration.nanoseconds / units[i].nanoseconds << ' ' << units[i].symbol);
  }

  Duration &operator+=(Duration const &rhs) noexcept {
    nanoseconds += rhs.nanoseconds;
    return *this;
  }

private:
  U64 nanoseconds = 0;
};

// A stopwatch that starts at construction.
class [[nodiscard]] Timer {
public:
  [[nodiscard]] Duration getElapsed() const noexcept {
    return Duration{checkedCast<U64>(std::chrono::duration_cast<std::chrono::nanoseconds>(now() - start).count())};
  }

private:
  using Clock = std::chrono::steady_clock;

  [[nodiscard]] Clock::time_point now() const noexcept {
    return Clock::now();
  }

  decltype(std::declval<Timer>().now()) start = now();
};
} // namespace AoC
