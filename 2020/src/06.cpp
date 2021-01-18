#include "ArgumentParser.hpp"
#include "IO.hpp"

#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

namespace AoC {
void main(ArgumentParser const &argumentParser) {
  auto const part = argumentParser.getPart();
  auto stream = std::ifstream(argumentParser.getPath());
  std::bitset<26> declared;
  if (part == 2) {
    declared.flip();
  }
  std::size_t count = 0;
  auto const flushDeclared = [part, &declared, &count]() {
    count += declared.count();
    declared.reset();
    if (part == 2) {
      declared.flip();
    }
  };
  std::string line;
  while (std::getline(stream, line)) {
    if (line.empty()) {
      flushDeclared();
      continue;
    }
    std::bitset<26> declaredInLine{};
    for (auto const character : line) {
      if ('a' <= character && character <= 'z') {
        declaredInLine[character - 'a'] = true;
      }
    }
    if (part == 1) {
      declared |= declaredInLine;
    } else {
      declared &= declaredInLine;
    }
  }
  flushDeclared();
  std::cout << count << '\n';
}
} // namespace AoC

#include "Main.inl"
