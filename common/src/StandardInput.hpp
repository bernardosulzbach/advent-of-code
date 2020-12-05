#pragma once

#include <vector>

namespace AoC {
template <typename T, typename Stream> std::vector<T> readVector(Stream &s) {
  std::vector<T> vector;
  T t;
  while (s >> t) {
    vector.push_back(t);
  }
  return vector;
}
} // namespace AoC