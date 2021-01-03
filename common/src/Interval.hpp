#pragma once

#include <algorithm>
#include <vector>

namespace AoC {
template <typename T> struct Interval {
  T start;
  T finish;

  /**
   * Constructs an interval from the specified endpoints.
   * @param intervalStart, the start of the interval, inclusive
   * @param intervalFinish, the end of the interval, noninclusive
   */
  Interval(T const intervalStart, T const intervalFinish) noexcept : start(intervalStart), finish(intervalFinish) {
  }

  [[nodiscard]] bool contains(T const value) const noexcept {
    return start <= value && value < finish;
  }

  [[nodiscard]] auto operator<=>(Interval const &) const noexcept = default;
};

template <typename Stream, typename T> Stream &operator<<(Stream &s, Interval<T> const interval) noexcept {
  return (s << "[" << interval.start << ", " << interval.finish << ")");
}

/**
 * Requires sorted intervals.
 */
template <typename T>
[[nodiscard]] bool inIntervalUnion(std::vector<Interval<T>> const &intervals, T const value) noexcept {
  return std::ranges::any_of(intervals, [value](auto const interval) { return interval.contains(value); });
}

/**
 * Requires sorted intervals.
 */
template <typename T> void mergeIntervals(std::vector<Interval<T>> &intervals) noexcept {
  if (intervals.size() < 2) {
    return;
  }
  auto write_it = std::begin(intervals);
  for (auto it = std::next(std::begin(intervals)); it != std::end(intervals); ++it) {
    if (it->start <= write_it->finish) {
      write_it->finish = std::max(write_it->finish, it->finish);
    } else {
      ++write_it;
      *write_it = *it; // Requires well-behaved self-assignment.
    }
  }
  intervals.erase(std::next(write_it), std::end(intervals));
}

/**
 * Requires merged intervals.
 *
 * Maintains the intervals merged.
 */
template <typename T> void insertInterval(std::vector<Interval<T>> &intervals, const Interval<T> interval) {
  auto insertion_iterator = std::upper_bound(std::begin(intervals), std::end(intervals), interval);
  const auto merge_from_insertion_iterator = [&intervals, &insertion_iterator]() {
    auto read_iterator = std::next(insertion_iterator);
    const auto begin_of_erase = read_iterator;
    while (read_iterator != std::end(intervals) && read_iterator->start <= insertion_iterator->finish) {
      insertion_iterator->finish = std::max(insertion_iterator->finish, read_iterator->finish);
      ++read_iterator;
    }
    const auto end_of_erase = read_iterator;
    intervals.erase(begin_of_erase, end_of_erase);
  };
  if (insertion_iterator != std::begin(intervals)) {
    --insertion_iterator;
    if (interval.start <= insertion_iterator->finish) {
      insertion_iterator->finish = std::max(insertion_iterator->finish, interval.finish);
      merge_from_insertion_iterator();
      return;
    }
    ++insertion_iterator;
  }
  if (insertion_iterator != std::end(intervals) && interval.finish >= insertion_iterator->start) {
    insertion_iterator->start = interval.start;
    insertion_iterator->finish = std::max(insertion_iterator->finish, interval.finish);
    merge_from_insertion_iterator();
    return;
  }
  intervals.insert(insertion_iterator, interval);
}

/**
 * Requires merged intervals.
 */
template <typename T> std::vector<Interval<T>> computeIntervalNegation(const std::vector<Interval<T>> &intervals) {
  std::vector<Interval<T>> negation = {{std::numeric_limits<int>::min(), std::numeric_limits<int>::max()}};
  for (const auto &interval : intervals) {
    std::rbegin(negation)->finish = interval.start;
    negation.emplace_back(interval.finish, std::numeric_limits<int>::max());
  }
  return negation;
}
} // namespace AoC