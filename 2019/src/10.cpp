#include "ArgumentParser.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Vector.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

constexpr F64 Epsilon = 1.0e-6;

using AsteroidField = std::vector<std::vector<bool>>;

struct VisibleAsteroid {
  std::size_t i;
  std::size_t j;
  Vector direction;

  [[nodiscard]] F64 getClockwiseAngle() const {
    auto angle = direction.getAngle();
    if (angle < 0.0) {
      angle += 2.0 * Pi;
    }
    if (angle <= 0.5 * Pi) {
      angle = 0.5 * Pi - angle;
    } else {
      angle = 0.5 * Pi + 2.0 * Pi - angle;
    }
    return angle;
  }

  [[nodiscard]] F64 getDistance(std::size_t oI, std::size_t oJ) const {
    const auto dI = static_cast<F64>(i) - oI;
    const auto dJ = static_cast<F64>(j) - oJ;
    return std::hypotf(dI, dJ);
  }
};

std::vector<VisibleAsteroid> computeVisibleAsteroids(const AsteroidField &asteroids, std::size_t cI, std::size_t cJ) {
  const auto cIAsFloat = static_cast<F64>(cI);
  const auto cJAsFloat = static_cast<F64>(cJ);
  std::vector<VisibleAsteroid> visibleAsteroids;
  for (std::size_t i = 0; i < asteroids.size(); i++) {
    for (std::size_t j = 0; j < asteroids[i].size(); j++) {
      if (i == cI && j == cJ) {
        continue;
      }
      if (!asteroids[i][j]) {
        continue;
      }
      const auto di = cIAsFloat - static_cast<F64>(i);
      const auto dj = static_cast<F64>(j) - cJAsFloat;
      const auto directionVector = Vector(dj, di).normalize();
      const auto None = std::numeric_limits<std::size_t>::max();
      auto kForDirection = None;
      for (std::size_t k = 0; k < visibleAsteroids.size(); k++) {
        if (visibleAsteroids[k].direction.subtract(directionVector).getNorm() < Epsilon) {
          kForDirection = k;
        }
      }
      if (kForDirection == None) {
        visibleAsteroids.push_back({i, j, directionVector});
      } else {
        visibleAsteroids[kForDirection] = VisibleAsteroid{i, j, directionVector};
      }
    }
  }
  return visibleAsteroids;
}

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  AsteroidField asteroids;
  {
    std::ifstream stream(path);
    std::string line;
    std::size_t i = 0;
    while (stream >> line) {
      asteroids.emplace_back(line.size(), false);
      for (std::size_t j = 0; j < line.size(); j++) {
        asteroids[i][j] = line[j] == '#';
      }
      i++;
    }
  }
  const auto part = argumentParser.getAdditionalArgument(0);
  std::size_t maximumVisible = 0;
  std::size_t iOfBestPosition = std::numeric_limits<std::size_t>::max();
  std::size_t jOfBestPosition = std::numeric_limits<std::size_t>::max();
  for (std::size_t i = 0; i < asteroids.size(); i++) {
    for (std::size_t j = 0; j < asteroids[i].size(); j++) {
      if (asteroids[i][j]) {
        const auto count = computeVisibleAsteroids(asteroids, i, j).size();
        if (count > maximumVisible) {
          maximumVisible = count;
          iOfBestPosition = i;
          jOfBestPosition = j;
        }
      }
    }
  }
  if (part == 1) {
    std::cout << maximumVisible << '\n';
  } else if (part == 2) {
    U32 destroyed = 0;
    std::optional<VisibleAsteroid> lastDestroyed;
    std::vector<VisibleAsteroid> visibleAsteroids;
    while (destroyed < 200) {
      if (visibleAsteroids.empty()) {
        visibleAsteroids = computeVisibleAsteroids(asteroids, iOfBestPosition, jOfBestPosition);
        std::sort(std::rbegin(visibleAsteroids), std::rend(visibleAsteroids),
                  [](const VisibleAsteroid &a, const VisibleAsteroid &b) {
                    return a.getClockwiseAngle() < b.getClockwiseAngle();
                  });
      }
      lastDestroyed = visibleAsteroids.back();
      visibleAsteroids.pop_back();
      asteroids[lastDestroyed->i][lastDestroyed->j] = false;
      destroyed++;
    }
    std::cout << lastDestroyed->i + 100 * lastDestroyed->j << '\n';
  } else {
    throw std::invalid_argument("Part should be 1 or 2.");
  }
  return 0;
}
