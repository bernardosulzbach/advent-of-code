#include "ArgumentParser.hpp"
#include "BugScan.hpp"
#include "Filesystem.hpp"

#include <cassert>
#include <iostream>
#include <unordered_set>

namespace AoC {
void main(ArgumentParser const &argumentParser) {
  const auto text = readFile(argumentParser.getPath());
  if (argumentParser.getPart() == 1) {
    BugScan scan = BugScan::fromString(text);
    std::unordered_set<BugScan> seen;
    while (seen.count(scan) == 0) {
      seen.insert(scan);
      scan = scan.evolve();
    }
    std::cout << scan.getBiodiversity() << '\n';
  } else {
    assert(false);
  }
}
} // namespace AoC

#include "Main.inl"
