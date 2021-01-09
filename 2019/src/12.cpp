#include "ArgumentParser.hpp"
#include "Body.hpp"
#include "Cycle.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Point.hpp"
#include "Vector.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

Vector<S64, 3> computeGravity(const std::vector<Body> &bodies, Vector<S64, 3> position) {
  Vector<S64, 3> gravity;
  for (const auto &body : bodies) {
    for (std::size_t i = 0; i < 3; i++) {
      if (body.position.components[i] < position.components[i]) {
        gravity.components[i]--;
      } else if (body.position.components[i] > position.components[i]) {
        gravity.components[i]++;
      }
    }
  }
  return gravity;
}

void updateBodies(std::vector<Body> &bodies) {
  std::vector<Vector<S64, 3>> gravities;
  gravities.reserve(bodies.size());
  for (const auto body : bodies) {
    gravities.push_back(computeGravity(bodies, body.position));
  }
  for (std::size_t i = 0; i < bodies.size(); i++) {
    auto &body = bodies[i];
    body.velocity = body.velocity + gravities[i];
    body.position = body.position + body.velocity;
  }
}

std::array<Cycle, 3> findCycles(const std::vector<Body> &initialBodies) {
  const auto Unknown = std::numeric_limits<S64>::max();
  std::array<Cycle, 3> cycles{Cycle{Unknown, Unknown}, Cycle{Unknown, Unknown}, Cycle{Unknown, Unknown}};
  const auto hasUnknownValues = [&cycles]() {
    return std::ranges::any_of(cycles,
                               [](auto const cycle) { return cycle.offset == Unknown || cycle.period == Unknown; });
  };
  auto bodies = initialBodies;
  for (S64 t = 0; hasUnknownValues(); t++) {
    updateBodies(bodies);
    for (std::size_t d = 0; d < 3; d++) {
      auto matchesFirst = true;
      for (std::size_t i = 0; i < bodies.size(); i++) {
        matchesFirst = matchesFirst && bodies[i].velocity.components[d] == initialBodies[i].velocity.components[d];
        matchesFirst = matchesFirst && bodies[i].position.components[d] == initialBodies[i].position.components[d];
      }
      if (matchesFirst) {
        if (cycles[d].offset == Unknown) {
          cycles[d].offset = t + 1;
        } else if (cycles[d].period == Unknown) {
          cycles[d].period = t - cycles[d].offset + 1;
        }
      }
    }
  }
  return cycles;
}

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  auto bodies = readBodies(path);
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part != 1 && part != 2) {
    throw std::invalid_argument("Part should be 1 or 2.");
  }
  if (part == 1) {
    for (U32 it = 1; it <= 1000; it++) {
      updateBodies(bodies);
    }
    S64 totalEnergy{};
    for (const auto &body : bodies) {
      totalEnergy += body.position.getL1Norm() * body.velocity.getL1Norm();
    }
    std::cout << totalEnergy << '\n';
  } else {
    const auto cycles = findCycles(bodies);
    std::cout << leastCommonMultiple(leastCommonMultiple(cycles[0].period, cycles[1].period), cycles[2].period) << '\n';
  }
  return 0;
}
