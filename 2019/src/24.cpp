#include "ArgumentParser.hpp"
#include "BugScan.hpp"
#include "Filesystem.hpp"

#include <cassert>
#include <iostream>
#include <unordered_set>

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part != 1 && part != 2) {
    throw std::invalid_argument("Part should be 1 or 2.");
  }

  const auto text = readFile(path);
  if (part == 1) {
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
  return 0;
}
