#include "ArgumentParser.hpp"
#include "Body.hpp"
#include "Cycle.hpp"
#include "DirectedGraph.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Point.hpp"
#include "Robot.hpp"
#include "Screen.hpp"
#include "Text.hpp"
#include "Vector.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

std::array<Point<S32>, 4> getNeighbors(Point<S32> point) {
  return {Point<S32>{point.x - 1, point.y}, Point<S32>{point.x, point.y - 1}, Point<S32>{point.x, point.y + 1},
          Point<S32>{point.x + 1, point.y}};
}

struct RobotPath {
  Point<S32> end;
  std::vector<Direction> directions;
};

class Map {
public:
  std::unordered_map<Point<S32>, TileState> tiles;

  [[nodiscard]] bool hasUnknownTiles() const {
    for (const auto &entry : tiles) {
      if (entry.second == TileState::Unknown) {
        return true;
      }
    }
    return false;
  }

  [[nodiscard]] bool is(Point<S32> point, TileState state) const {
    return tiles.count(point) > 0 && tiles.at(point) == state;
  }

  RobotPath getPathToUnknown(Point<S32> origin) const {
    std::queue<RobotPath> paths;
    std::unordered_set<Point<S32>> visitedPoints;
    paths.push({origin, {}});
    visitedPoints.insert(origin);
    while (!paths.empty()) {
      const auto path = paths.front();
      paths.pop();
      if (is(path.end, TileState::Unknown)) {
        return path;
      }
      for (const auto &neighbor : getNeighbors(path.end)) {
        if (visitedPoints.count(neighbor) > 0) {
          continue;
        }
        if (is(neighbor, TileState::Empty) || is(neighbor, TileState::Unknown)) {
          visitedPoints.insert(neighbor);
          auto pathToNeighbor = path;
          pathToNeighbor.end = neighbor;
          if (neighbor.y > path.end.y) {
            pathToNeighbor.directions.push_back(Direction::North);
          } else if (neighbor.x > path.end.x) {
            pathToNeighbor.directions.push_back(Direction::East);
          } else if (neighbor.y < path.end.y) {
            pathToNeighbor.directions.push_back(Direction::South);
          } else if (neighbor.x < path.end.x) {
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

  S32 getDistanceBetween(const Point<S32> a, const Point<S32> b) {
    struct Option {
      Point<S32> destination;
      S32 steps;
    };
    std::queue<Option> options;
    std::unordered_set<Point<S32>> visitedPoints;
    options.push({a, 0});
    visitedPoints.insert(a);
    while (!options.empty()) {
      const auto option = options.front();
      options.pop();
      if (option.destination == b) {
        return option.steps;
      }
      for (const auto &neighbor : getNeighbors(option.destination)) {
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

  S32 getStepsToFloodFrom(const Point<S32> origin) const {
    struct Option {
      Point<S32> destination;
      S32 steps;
    };
    std::queue<Option> options;
    std::unordered_set<Point<S32>> visitedPoints;
    options.push({origin, 0});
    visitedPoints.insert(origin);
    S32 maximumSteps = 0;
    while (!options.empty()) {
      const auto option = options.front();
      options.pop();
      maximumSteps = std::max(maximumSteps, option.steps);
      for (const auto &neighbor : getNeighbors(option.destination)) {
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

  void addEmpty(Point<S32> point) {
    assert(tiles[point] == TileState::Unknown);
    tiles[point] = TileState::Empty;
    for (const auto &neighbor : getNeighbors(point)) {
      if (tiles.count(neighbor) == 0) {
        tiles[neighbor] = TileState::Unknown;
      }
    }
  }

  void addWall(Point<S32> point) {
    assert(tiles[point] == TileState::Unknown);
    tiles[point] = TileState::Wall;
  }
};

std::ostream &operator<<(std::ostream &stream, const Map &map) {
  const auto max = std::numeric_limits<S32>::max();
  const auto min = std::numeric_limits<S32>::min();
  Point<S32> topLeft{max, min};
  Point<S32> bottomRight{min, max};
  for (const auto &entry : map.tiles) {
    topLeft.x = std::min(topLeft.x, entry.first.x);
    topLeft.y = std::max(topLeft.y, entry.first.y);
    bottomRight.x = std::max(bottomRight.x, entry.first.x);
    bottomRight.y = std::min(bottomRight.y, entry.first.y);
  }
  for (auto y = topLeft.y; y >= bottomRight.y; y--) {
    for (auto x = topLeft.x; x <= bottomRight.x; x++) {
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

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part != 1 && part != 2) {
    throw std::invalid_argument("Part should be 1 or 2.");
  }

  Intcode intcode(readMemory(path));
  std::optional<Point<S32>> oxygenSystemPosition;
  const Point<S32> startingRobotPosition{0, 0};
  auto robotPosition = startingRobotPosition;
  const auto updateRobotPosition = [&robotPosition](Direction direction) {
    switch (direction) {
    case Direction::North:
      robotPosition.y++;
      break;
    case Direction::East:
      robotPosition.x++;
      break;
    case Direction::South:
      robotPosition.y--;
      break;
    case Direction::West:
      robotPosition.x--;
      break;
    }
  };
  Map map;
  map.addEmpty(robotPosition);
  while (map.hasUnknownTiles()) {
    const auto robotPath = map.getPathToUnknown(robotPosition);
    for (std::size_t i = 0; i < robotPath.directions.size(); i++) {
      if (i > 0) {
        assert(intcode.getOutput() != 0);
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

  if (part == 1) {
    std::cout << map.getDistanceBetween(startingRobotPosition, oxygenSystemPosition.value()) << '\n';
  } else {
    std::cout << map.getStepsToFloodFrom(oxygenSystemPosition.value()) << '\n';
  }

  return 0;
}
