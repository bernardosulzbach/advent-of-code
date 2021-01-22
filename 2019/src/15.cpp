#include "ArgumentParser.hpp"
#include "Body.hpp"
#include "Cycle.hpp"
#include "DirectedGraph.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Point.hpp"
#include "Screen.hpp"
#include "Text.hpp"
#include "Vector.hpp"
#include "Verify.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace AoC {
enum class TileState { Unknown, Empty, Wall };

char getTileStateSymbol(TileState tileState) {
  switch (tileState) {
  case TileState::Unknown:
    return '~';
  case TileState::Empty:
    return '.';
  case TileState::Wall:
    return '#';
  }
  throw std::runtime_error("Should not happen.");
}

struct RobotPath {
  Point<S32, 2> end;
  std::vector<Direction> directions;
};

class Map {
public:
  std::unordered_map<Point<S32, 2>, TileState> tiles;

  [[nodiscard]] bool hasUnknownTiles() const {
    return std::ranges::any_of(tiles, [](auto const entry) { return entry.second == TileState::Unknown; });
  }

  [[nodiscard]] bool is(Point<S32, 2> const point, TileState const state) const {
    return tiles.count(point) > 0 && tiles.at(point) == state;
  }

  RobotPath getPathToUnknown(Point<S32, 2> const origin) const {
    std::queue<RobotPath> paths;
    std::unordered_set<Point<S32, 2>> visitedPoints;
    paths.push({origin, {}});
    visitedPoints.insert(origin);
    while (!paths.empty()) {
      const auto path = paths.front();
      paths.pop();
      if (is(path.end, TileState::Unknown)) {
        return path;
      }
      for (const auto &neighbor : path.end.getDirectNeighbors()) {
        if (visitedPoints.count(neighbor) > 0) {
          continue;
        }
        if (is(neighbor, TileState::Empty) || is(neighbor, TileState::Unknown)) {
          visitedPoints.insert(neighbor);
          auto pathToNeighbor = path;
          pathToNeighbor.end = neighbor;
          if (neighbor.getY() > path.end.getY()) {
            pathToNeighbor.directions.push_back(Direction::North);
          } else if (neighbor.getX() > path.end.getX()) {
            pathToNeighbor.directions.push_back(Direction::East);
          } else if (neighbor.getY() < path.end.getY()) {
            pathToNeighbor.directions.push_back(Direction::South);
          } else if (neighbor.getX() < path.end.getX()) {
            pathToNeighbor.directions.push_back(Direction::West);
          } else {
            assert(false);
          }
          paths.push(pathToNeighbor);
        }
      }
    }
    throw std::runtime_error("Could not find a path.");
  }

  S32 getDistanceBetween(Point<S32, 2> const a, Point<S32, 2> const b) {
    struct Option {
      Point<S32, 2> destination;
      S32 steps;
    };
    std::queue<Option> options;
    std::unordered_set<Point<S32, 2>> visitedPoints;
    options.push({a, 0});
    visitedPoints.insert(a);
    while (!options.empty()) {
      const auto option = options.front();
      options.pop();
      if (option.destination == b) {
        return option.steps;
      }
      for (const auto &neighbor : option.destination.getDirectNeighbors()) {
        if (visitedPoints.count(neighbor) > 0) {
          continue;
        }
        if (is(neighbor, TileState::Empty)) {
          visitedPoints.insert(neighbor);
          auto optionToNeighbor = option;
          optionToNeighbor.destination = neighbor;
          optionToNeighbor.steps++;
          options.push(optionToNeighbor);
        }
      }
    }
    throw std::runtime_error("Could not find a path.");
  }

  S32 getStepsToFloodFrom(Point<S32, 2> const origin) const {
    struct Option {
      Point<S32, 2> destination;
      S32 steps;
    };
    std::queue<Option> options;
    std::unordered_set<Point<S32, 2>> visitedPoints;
    options.push({origin, 0});
    visitedPoints.insert(origin);
    S32 maximumSteps = 0;
    while (!options.empty()) {
      const auto option = options.front();
      options.pop();
      maximumSteps = std::max(maximumSteps, option.steps);
      for (const auto &neighbor : option.destination.getDirectNeighbors()) {
        if (visitedPoints.count(neighbor) > 0) {
          continue;
        }
        if (is(neighbor, TileState::Empty)) {
          visitedPoints.insert(neighbor);
          auto optionToNeighbor = option;
          optionToNeighbor.destination = neighbor;
          optionToNeighbor.steps++;
          options.push(optionToNeighbor);
        }
      }
    }
    return maximumSteps;
  }

  void addEmpty(Point<S32, 2> const point) {
    assert(tiles[point] == TileState::Unknown);
    tiles[point] = TileState::Empty;
    for (const auto &neighbor : point.getDirectNeighbors()) {
      if (tiles.count(neighbor) == 0) {
        tiles[neighbor] = TileState::Unknown;
      }
    }
  }

  void addWall(Point<S32, 2> const point) {
    assert(tiles[point] == TileState::Unknown);
    tiles[point] = TileState::Wall;
  }
};

std::ostream &operator<<(std::ostream &stream, const Map &map) {
  const auto max = std::numeric_limits<S32>::max();
  const auto min = std::numeric_limits<S32>::min();
  Point<S32, 2> topLeft{max, min};
  Point<S32, 2> bottomRight{min, max};
  for (const auto &entry : map.tiles) {
    topLeft.getX() = std::min(topLeft.getX(), entry.first.getX());
    topLeft.getY() = std::max(topLeft.getY(), entry.first.getY());
    bottomRight.getX() = std::max(bottomRight.getX(), entry.first.getX());
    bottomRight.getY() = std::min(bottomRight.getY(), entry.first.getY());
  }
  for (auto y = topLeft.getY(); y >= bottomRight.getY(); y--) {
    for (auto x = topLeft.getX(); x <= bottomRight.getX(); x++) {
      if (map.tiles.count({x, y}) != 0) {
        const auto state = map.tiles.at({x, y});
        stream << getTileStateSymbol(state);
      } else {
        stream << ' ';
      }
    }
    stream << '\n';
  }
  return stream;
}

Intcode::ValueType directionToCommand(Direction direction) {
  switch (direction) {
  case Direction::North:
    return 1;
  case Direction::East:
    return 4;
  case Direction::South:
    return 2;
  case Direction::West:
    return 3;
  }
  throw std::runtime_error("Not all cases are handled.");
}

void main(ArgumentParser const &argumentParser) {
  Intcode intcode(readMemory(argumentParser.getPath()));
  std::optional<Point<S32, 2>> oxygenSystemPosition;
  const Point<S32, 2> startingRobotPosition{0, 0};
  auto robotPosition = startingRobotPosition;
  const auto updateRobotPosition = [&robotPosition](Direction direction) {
    switch (direction) {
    case Direction::North:
      robotPosition.getY()++;
      break;
    case Direction::East:
      robotPosition.getX()++;
      break;
    case Direction::South:
      robotPosition.getY()--;
      break;
    case Direction::West:
      robotPosition.getX()--;
      break;
    }
  };
  Map map;
  map.addEmpty(robotPosition);
  while (map.hasUnknownTiles()) {
    const auto robotPath = map.getPathToUnknown(robotPosition);
    for (std::size_t i = 0; i < robotPath.directions.size(); i++) {
      if (i > 0) {
        verify(intcode.getOutput() != 0);
        updateRobotPosition(robotPath.directions[i - 1]);
      }
      intcode.addInput(directionToCommand(robotPath.directions[i]));
      intcode.run();
    }
    const auto result = intcode.getOutput();
    if (result == 0) {
      map.addWall(robotPath.end);
    } else if (result == 1 || result == 2) {
      updateRobotPosition(robotPath.directions.back());
      map.addEmpty(robotPath.end);
      if (result == 2) {
        oxygenSystemPosition = robotPosition;
      }
    } else {
      assert(false);
    }
  }
  assert(oxygenSystemPosition.has_value());
  if (argumentParser.getPart() == 1) {
    std::cout << map.getDistanceBetween(startingRobotPosition, oxygenSystemPosition.value()) << '\n';
  } else {
    std::cout << map.getStepsToFloodFrom(oxygenSystemPosition.value()) << '\n';
  }
}
} // namespace AoC

#include "Main.inl"
