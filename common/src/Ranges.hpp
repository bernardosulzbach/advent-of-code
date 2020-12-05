#pragma once

#include <cassert>
#include <tuple>
#include <type_traits>
#include <utility>

namespace AoC {
template <typename Range> struct PairIterator {
  using RangeIter = decltype(std::declval<Range>().cbegin());
  using RangeType = std::decay_t<decltype(*std::declval<RangeIter>())>;

  RangeIter it1;
  RangeIter it2;
  RangeIter itEnd;

  explicit PairIterator(Range const &range) : it1(range.cbegin()), it2(range.cbegin()), itEnd(range.cend()) {
    if (it2 != itEnd) {
      ++it2;
      if (it2 == itEnd) {
        toEnd();
      }
    }
  }

  PairIterator &operator++() &noexcept {
    assert(it1 != itEnd);
    assert(it2 != itEnd);
    ++it2;
    if (it2 == itEnd) {
      ++it1; // Cannot become itEnd, as it was behind it2, which wasn't itEnd.
      it2 = std::next(it1);
      if (it2 == itEnd) {
        toEnd();
      }
    }
    return *this;
  }

  // Sets all iterators to itEnd.
  void toEnd() &noexcept {
    it1 = it2 = itEnd;
  }

  [[nodiscard]] bool operator==(PairIterator const &rhs) const &noexcept {
    return std::tie(it1, it2) == std::tie(rhs.it1, rhs.it2);
  }

  [[nodiscard]] bool operator!=(PairIterator const &rhs) const &noexcept {
    return !(*this == rhs);
  }

  std::pair<RangeType, RangeType> operator*() const &noexcept {
    assert(it1 != itEnd);
    assert(it2 != itEnd);
    return std::make_pair(*it1, *it2);
  }
};

template <typename Range> struct PairIteratorRange {
  PairIterator<Range> itBegin;
  PairIterator<Range> itEnd;

  explicit PairIteratorRange(Range const &range) : itBegin(range), itEnd(range) {
    itEnd.toEnd();
  }

  PairIterator<Range> begin() const &noexcept {
    return itBegin;
  }

  PairIterator<Range> end() const &noexcept {
    return itEnd;
  }
};

template <typename Range> decltype(auto) allPairs(Range const &range) {
  return PairIteratorRange(range);
}
} // namespace AoC