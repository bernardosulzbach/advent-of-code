#include "ArgumentParser.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Vector.hpp"

#include <iostream>
#include <optional>
#include <vector>

namespace AoC {
constexpr F64 Epsilon = 1.0e-6;

using AsteroidField = std::vector<std::vector<bool>>;

struct VisibleAsteroid {
  std::size_t i;
  std::size_t j;
  Vector<F64, 2> direction;

  [[nodiscard]] F64 getClockwiseAngle() const {
    auto angle = getAngle(direction);
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

  [[nodiscard]] F64 getDistance(std::size_t const oI, std::size_t const oJ) const {
    const auto dI = static_cast<F64>(i) - static_cast<F64>(oI);
    const auto dJ = static_cast<F64>(j) - static_cast<F64>(oJ);
    return std::hypot(dI, dJ);
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
      const auto directionVector = Vector<F64, 2>{dj, di}.normalize();
      const auto None = std::numeric_limits<std::size_t>::max();
      auto kForDirection = None;
      for (std::size_t k = 0; k < visibleAsteroids.size(); k++) {
        if ((visibleAsteroids[k].direction - directionVector).getL2Norm() < Epsilon) {
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

void main(std::istream &stream, U32 const part) {
  AsteroidField asteroids;
  {
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
  } else {
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
  }
}
} // namespace AoC

#include "Main.inl"
