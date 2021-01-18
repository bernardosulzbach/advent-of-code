#include "Text.hpp"
#include "Math.hpp"

#include <cassert>
#include <iomanip>
#include <sstream>

std::vector<std::string> splitAt(std::string_view const string, std::string_view const separator) {
  assert(!separator.empty());
  std::vector<std::string> fragments;
  std::size_t start = 0;
  std::size_t result;
  while ((result = string.find(separator, start)) != std::string::npos) {
    fragments.emplace_back(string.substr(start, result - start));
    start = result + separator.size();
  }
  fragments.emplace_back(string.substr(start));
  return fragments;
}

namespace AoC {
std::string toZeroPaddedString(U64 day, U64 length) {
  std::stringstream stream;
  stream << std::setfill('0') << std::setw(checkedCast<int>(length)) << day;
  return stream.str();
}
} // namespace AoC