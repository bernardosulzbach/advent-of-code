#pragma once

#include <boost/functional/hash/hash.hpp>

namespace AoC {
struct Hash {
  template <typename T> [[nodiscard]] explicit Hash(T const &t) noexcept : hash(boost::hash<T>()(t)) {
  }

  void combine(Hash const rhs) noexcept {
    boost::hash_combine(hash, rhs.hash);
  }

  [[nodiscard]] constexpr std::size_t toStandardSize() const noexcept {
    return hash;
  }

private:
  std::size_t hash{};
};
} // namespace AoC