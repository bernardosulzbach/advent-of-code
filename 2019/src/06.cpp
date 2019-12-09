#include "ArgumentParser.hpp"
#include "OrbitGraph.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  OrbitGraph orbitGraph;
  {
    std::ifstream stream(path);
    std::string line;
    while (stream >> line) {
      const auto operatorPosition = line.find(')');
      const auto a = line.substr(0, operatorPosition);
      const auto b = line.substr(operatorPosition + 1);
      orbitGraph.addOrbit(a, b);
    }
  }
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part == 1) {
    std::cout << orbitGraph.getTotalOrbits() << '\n';
  } else if (part == 2) {
    std::cout << orbitGraph.distanceBetween("YOU", "SAN") - 2 << '\n';
  } else {
    throw std::runtime_error("Part should be 1 or 2.");
  }
  return 0;
}
