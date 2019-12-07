#include "ArgumentParser.hpp"
#include "OrbitGraph.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  const auto path = parseArguments(argc, argv);
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
  std::cout << orbitGraph.getTotalOrbits() << '\n';
  std::cout << orbitGraph.distanceBetween("YOU", "SAN") - 2 << '\n';
  return 0;
}
