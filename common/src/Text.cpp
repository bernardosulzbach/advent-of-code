#include "Text.hpp"

#include <cassert>

std::vector<std::string> splitAt(const std::string &string, const std::string &separator) {
  assert(!separator.empty());
  std::vector<std::string> fragments;
  std::size_t start = 0;
  std::size_t result;
  while ((result = string.find(separator, start)) != std::string::npos) {
    fragments.push_back(string.substr(start, result - start));
    start = result + separator.size();
  }
  fragments.push_back(string.substr(start));
  return fragments;
}
