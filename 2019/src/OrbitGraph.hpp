#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "Types.hpp"

class OrbitGraph {
  constexpr static auto Undefined = std::numeric_limits<U64>::max();

private:
  U64 computeTotalOrbits(std::vector<U64> &totalOrbits, U32 index) const {
    if (totalOrbits[index] != Undefined) {
      return totalOrbits[index];
    }
    U64 totalOrbitsOfIndex = 0;
    for (const auto orbitedBy : orbits[index]) {
      totalOrbitsOfIndex += computeTotalOrbits(totalOrbits, orbitedBy) + 1;
    }
    totalOrbits[index] = totalOrbitsOfIndex;
    return totalOrbits[index];
  }

public:
  std::unordered_map<std::string, U32> labelToIndex;
  std::vector<std::string> labels;
  std::vector<std::vector<U32>> orbits;

  U32 getIndex(const std::string &label) {
    if (labelToIndex.count(label) == 0) {
      labelToIndex[label] = labels.size();
      labels.push_back(label);
    }
    return labelToIndex[label];
  }

  void addOrbit(const std::string &a, const std::string &b) {
    const auto aIndex = getIndex(a);
    const auto bIndex = getIndex(b);
    if (bIndex >= orbits.size()) {
      orbits.resize(bIndex + 1);
    }
    orbits[bIndex].push_back(aIndex);
  }

  U64 getDirectOrbits() const {
    U64 directOrbits = 0;
    for (const auto &orbitCollection : orbits) {
      directOrbits += orbitCollection.size();
    }
    return directOrbits;
  }

  U64 getTotalOrbits() const {
    std::vector<U64> totalOrbits(orbits.size(), Undefined);
    U64 totalOrbitCount = 0;
    for (U64 i = 0; i < orbits.size(); i++) {
      totalOrbitCount += computeTotalOrbits(totalOrbits, i);
    }
    return totalOrbitCount;
  }

  std::vector<U64> distanceFromNode(const std::string &label) {
    const auto index = getIndex(label);
    std::vector<U64> distanceVector(orbits.size(), Undefined);
    struct Node {
      U32 index;
      U64 distance;
    };
    std::queue<Node> nodes;
    nodes.push({index, 0});
    while (!nodes.empty()) {
      const auto node = nodes.front();
      nodes.pop();
      distanceVector[node.index] = std::min(distanceVector[node.index], node.distance);
      for (const auto orbitedByNode : orbits[node.index]) {
        nodes.push({orbitedByNode, node.distance + 1});
      }
    }
    return distanceVector;
  }

  U64 distanceBetween(const std::string &a, const std::string &b) {
    const auto distanceFromA = distanceFromNode(a);
    const auto distanceFromB = distanceFromNode(b);
    auto best = Undefined;
    for (U64 i = 0; i < distanceFromA.size(); i++) {
      if (distanceFromA[i] == Undefined || distanceFromB[i] == Undefined) {
        continue;
      }
      best = std::min(best, distanceFromA[i] + distanceFromB[i]);
    }
    return best;
  }
};
