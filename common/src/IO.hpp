#pragma once

#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace AoC {
template <typename T, typename Stream> T readValue(Stream &s) {
  if (!s) {
    throw std::runtime_error("Stream is not in a good state.");
  }
  T t;
  if (s >> t) {
    return t;
  }
  throw std::runtime_error("Failed to read value from stream.");
}

template <typename T> T parseValue(std::string const &string) {
  std::stringstream stream(string);
  return readValue<T>(stream);
}

template <typename T, typename Stream> std::vector<T> readVector(Stream &s) {
  if (!s) {
    throw std::runtime_error("Stream is not in a good state.");
  }
  std::vector<T> vector;
  T t;
  while (s >> t) {
    vector.push_back(t);
  }
  return vector;
}

template <typename Stream, typename Range> void print(Stream &s, Range const &range, std::string_view const separator) {
  auto first = true;
  for (auto const &value : range) {
    if (!first) {
      s << separator;
    }
    s << value;
    first = false;
  }
  s << '\n';
}
} // namespace AoC