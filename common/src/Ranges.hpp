#pragma once

#include <cassert>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <utility>

namespace AoC {
template <typename Range>
using RangeType = std::decay_t<decltype(*std::declval<decltype(std::declval<Range>().begin())>())>;

template <typename Range> decltype(auto) sum(Range const &range) noexcept {
  return std::accumulate(std::begin(range), std::end(range), static_cast<RangeType<Range>>(0));
}

template <typename Range> decltype(auto) product(Range const &range) noexcept {
  return std::accumulate(std::begin(range), std::end(range), static_cast<RangeType<Range>>(1),
                         std::multiplies<RangeType<Range>>());
}

template <std::size_t N, typename Range> struct CombinationIterator {
  static_assert(N > 0);

  using RangeIter = decltype(std::declval<Range>().cbegin());

  std::array<RangeIter, N> iters;
  RangeIter itEnd;

  explicit CombinationIterator(Range const &range) : itEnd(range.end()) {
    for (decltype(N) i = 0; i < N; i++) {
      if (i == 0) {
        iters[i] = range.begin();
      } else {
        iters[i] = std::next(iters[i - 1]);
      }
      if (iters[i] == itEnd) {
        toEnd();
        return;
      }
    }
  }

  CombinationIterator &operator++() &noexcept {
    increment(N - 1);
    return *this;
  }

  // Sets all iterators to itEnd.
  void toEnd() &noexcept {
    std::fill(std::begin(iters), std::end(iters), itEnd);
  }

  [[nodiscard]] bool operator==(CombinationIterator const &rhs) const &noexcept {
    return std::tie(iters, itEnd) == std::tie(rhs.iters, rhs.itEnd);
  }

  [[nodiscard]] bool operator!=(CombinationIterator const &rhs) const &noexcept {
    return !(*this == rhs);
  }

  /**
   * Returns an array with the current combination by value.
   */
  std::array<RangeType<Range>, N> operator*() const &noexcept {
    std::array<RangeType<Range>, N> array;
    for (decltype(N) i = 0; i < N; i++) {
      assert(iters[i] != itEnd);
      array[i] = *iters[i];
    }
    return array;
  }

private:
  void increment(decltype(N) const i) &noexcept {
    ++iters[i];
    if (std::distance(iters[i], itEnd) == N - 1 - i) {
      if (i == 0) {
        toEnd();
        return;
      }
      increment(i - 1);
      if (iters[i - 1] != itEnd) {
        iters[i] = std::next(iters[i - 1]);
      }
    }
  }
};

template <std::size_t N, typename Range> struct CombinationIteratorRange {
  CombinationIterator<N, Range> itBegin;
  CombinationIterator<N, Range> itEnd;

  explicit CombinationIteratorRange(Range const &range) : itBegin(range), itEnd(range) {
    itEnd.toEnd();
  }

  CombinationIterator<N, Range> begin() const &noexcept {
    return itBegin;
  }

  CombinationIterator<N, Range> end() const &noexcept {
    return itEnd;
  }
};

template <std::size_t N, typename Range> decltype(auto) allCombinations(Range const &range) {
  return CombinationIteratorRange<N, Range>(range);
}
} // namespace AoC