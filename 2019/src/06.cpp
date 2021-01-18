#include "ArgumentParser.hpp"
#include "OrbitGraph.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace AoC {
void main(ArgumentParser const &argumentParser) {
  OrbitGraph orbitGraph;
  {
    std::ifstream stream(argumentParser.getPath());
    std::string line;
    while (stream >> line) {
      const auto operatorPosition = line.find(')');
      const auto a = line.substr(0, operatorPosition);
      const auto b = line.substr(operatorPosition + 1);
      orbitGraph.addOrbit(a, b);
    }
  }
  const auto part = argumentParser.getPart();
  if (part == 1) {
    std::cout << orbitGraph.getTotalOrbits() << '\n';
  } else {
    std::cout << orbitGraph.distanceBetween("YOU", "SAN") - 2 << '\n';
  }
}
} // namespace AoC

#include "Main.inl"
