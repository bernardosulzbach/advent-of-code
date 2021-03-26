#include "ArgumentParser.hpp"
#include "OrbitGraph.hpp"

#include <iostream>
#include <string>

namespace AoC {
void main(std::istream &stream, U32 const part) {
  OrbitGraph orbitGraph;
  {
    std::string line;
    while (stream >> line) {
      const auto operatorPosition = line.find(')');
      const auto a = line.substr(0, operatorPosition);
      const auto b = line.substr(operatorPosition + 1);
      orbitGraph.addOrbit(a, b);
    }
  }
  if (part == 1) {
    std::cout << orbitGraph.getTotalOrbits() << '\n';
  } else {
    std::cout << orbitGraph.distanceBetween("YOU", "SAN") - 2 << '\n';
  }
}
} // namespace AoC

#include "Main.inl"
