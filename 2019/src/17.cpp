#include "ArgumentParser.hpp"
#include "Cycle.hpp"
#include "Intcode.hpp"
#include "Point.hpp"
#include "Robot.hpp"
#include "Text.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <vector>

struct RobotMovement {
  bool turnedLeft = false;
  bool turnedRight = false;
  S32 stride = 0;
};

class Map {
public:
  std::vector<std::vector<char>> grid;
  std::optional<Robot> robot;
  std::vector<RobotMovement> robotMovements;

  void setRobot(Robot newRobot) {
    robot = newRobot;
  }

  [[nodiscard]] char getCharacter(Point<S32> point) const {
    if (robot && robot->getPosition() == point) {
      return directionToArrowCharacter(robot->getDirection());
    }
    return grid[static_cast<S32>(grid.size()) - 1 - point.y][point.x];
  }

  [[nodiscard]] bool hasSupport(Point<S32> point) const {
    const auto n = static_cast<S32>(grid.size());
    const auto m = static_cast<S32>(grid.front().size());
    if (point.x < 0 || point.x >= m || point.y < 0 || point.y >= n) {
      return false;
    }
    return getCharacter(point) == '#';
  }

  [[nodiscard]] bool hasIntersection(Point<S32> point) const {
    const auto n = static_cast<S32>(grid.size());
    const auto m = static_cast<S32>(grid.front().size());
    if (point.x < 1 || point.x >= m - 1 || point.y < 1 || point.y >= n - 1) {
      return false;
    }
    auto result = hasSupport(point);
    result = result && hasSupport(point.move(Direction::North));
    result = result && hasSupport(point.move(Direction::East));
    result = result && hasSupport(point.move(Direction::South));
    result = result && hasSupport(point.move(Direction::West));
    return result;
  }

  [[nodiscard]] std::vector<Point<S32>> getIntersections() const {
    std::vector<Point<S32>> intersections;
    const auto n = static_cast<S32>(grid.size());
    const auto m = static_cast<S32>(grid.front().size());
    for (S32 y = 0; y < n; y++) {
      for (S32 x = 0; x < m; x++) {
        if (hasIntersection(Point<S32>(x, y))) {
          intersections.emplace_back(x, y);
        }
      }
    }
    return intersections;
  }

  [[nodiscard]] bool canMoveTheRobotForward() const {
    auto robotCopy = *robot;
    robotCopy.goForward();
    return hasSupport(robotCopy.getPosition());
  }

  void moveTheRobotForward() {
    robot->goForward();
    if (!robotMovements.empty() && !robotMovements.back().turnedLeft && !robotMovements.back().turnedRight) {
      robotMovements.back().stride++;
    } else {
      robotMovements.push_back({false, false, 1});
    }
  }

  [[nodiscard]] bool canMoveTheRobotToTheRight() const {
    auto robotCopy = *robot;
    robotCopy.rotateRight();
    robotCopy.goForward();
    return hasSupport(robotCopy.getPosition());
  }

  void moveTheRobotToTheRight() {
    robot->rotateRight();
    robotMovements.push_back({false, true, 0});
    moveTheRobotForward();
  }

  [[nodiscard]] bool canMoveTheRobotToTheLeft() const {
    auto robotCopy = *robot;
    robotCopy.rotateLeft();
    robotCopy.goForward();
    return hasSupport(robotCopy.getPosition());
  }

  void moveTheRobotToTheLeft() {
    robot->rotateLeft();
    robotMovements.push_back({true, false, 0});
    moveTheRobotForward();
  }

  void dumpRobotMovement() {
    auto first = true;
    for (const auto movement : robotMovements) {
      if (!first) {
        std::cout << ", ";
      }
      if (movement.turnedLeft) {
        std::cout << 'L';
      } else if (movement.turnedRight) {
        std::cout << 'R';
      } else {
        std::cout << movement.stride;
      }
      first = false;
    }
    std::cout << "\n";
  }
};

std::ostream &operator<<(std::ostream &stream, const Map &map) {
  const auto n = static_cast<S32>(map.grid.size());
  const auto m = static_cast<S32>(map.grid.front().size());
  for (S32 y = n - 1; y >= 0; y--) {
    if (y < n - 1) {
      stream << '\n';
    }
    for (S32 x = 0; x < m; x++) {
      stream << map.getCharacter(Point<S32>(x, y));
    }
  }
  return stream;
}

Map readMap(Intcode &intcode) {
  intcode.run();
  Robot robot;
  std::optional<Point<S32>> robotOffset;
  Map map;
  map.grid.emplace_back();
  auto running = true;
  while (running) {
    const auto value = intcode.getOutput();
    switch (value) {
    case '#': {
      map.grid.back().push_back('#');
      break;
    }
    case '.': {
      map.grid.back().push_back('.');
      break;
    }
    case '\n': {
      if (!map.grid.back().empty()) {
        map.grid.emplace_back();
      } else {
        running = false;
      }
      break;
    }
    case '^':
    case 'v':
    case '<':
    case '>': {
      map.grid.back().push_back('#');
      const auto y = static_cast<S32>(map.grid.size());
      const auto x = static_cast<S32>(map.grid.back().size() - 1);
      robotOffset = Point<S32>(x, y);
      if (value == '^') {
        robot.setDirection(Direction::North);
      } else if (value == 'v') {
        robot.setDirection(Direction::South);
      } else if (value == '<') {
        robot.setDirection(Direction::West);
      } else {
        robot.setDirection(Direction::East);
      }
      break;
    }
    case 'X': {
      throw std::runtime_error("The robot fell.");
    }
    default: {
      throw std::runtime_error("Unmatched symbol: '" + std::string(1, value) + "'.");
    }
    }
  }
  map.grid.pop_back();
  assert(robotOffset.has_value());
  const auto robotPosition = Point<S32>(robotOffset->x, map.grid.size() - robotOffset->y);
  robot.setPosition(robotPosition);
  map.setRobot(robot);
  return map;
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
  if (part == 1) {
    const auto map = readMap(intcode);
    assert(!intcode.hasOutput());
    S32 total = 0;
    for (const auto intersection : map.getIntersections()) {
      total += (static_cast<S32>(map.grid.size()) - 1 - intersection.y) * intersection.x;
    }
    std::cout << total << '\n';
  } else {
    intcode.memory[0] = 2;
    const auto firstState = intcode.run();
    assert(firstState == IntcodeState::Blocked);
    auto initialMap = readMap(intcode);
    std::cout << initialMap << '\n' << '\n';
    while (true) {
      if (initialMap.canMoveTheRobotForward()) {
        initialMap.moveTheRobotForward();
      } else if (initialMap.canMoveTheRobotToTheLeft()) {
        initialMap.moveTheRobotToTheLeft();
      } else if (initialMap.canMoveTheRobotToTheRight()) {
        initialMap.moveTheRobotToTheRight();
      } else {
        break;
      }
    }
    initialMap.dumpRobotMovement();
    const auto enterInputString = [&intcode](const std::string &string) {
      while (intcode.hasOutput()) {
        intcode.getOutput();
      }
      for (const auto character : string) {
        intcode.addInput(character);
      }
      intcode.run();
    };
    const auto main = "A,B,A,C,A,B,C,B,C,B\n";
    enterInputString(main);
    const auto routineA = "R,8,L,10,L,12,R,4\n";
    enterInputString(routineA);
    const auto routineB = "R,8,L,12,R,4,R,4\n";
    enterInputString(routineB);
    const auto routineC = "R,8,L,10,R,8\n";
    enterInputString(routineC);
    enterInputString("y\n");
    intcode.getOutput();
    while (intcode.getOutputBufferLength() > 1) {
      std::cout << readMap(intcode) << '\n' << '\n';
    }
    std::cout << intcode.getOutput() << '\n';
  }

  return 0;
}
