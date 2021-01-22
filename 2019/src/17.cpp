#include "ArgumentParser.hpp"
#include "Cycle.hpp"
#include "Intcode.hpp"
#include "Point.hpp"
#include "Pose.hpp"
#include "Text.hpp"
#include "Verify.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <optional>
#include <variant>
#include <vector>

namespace AoC {
class RobotMovement {
public:
  bool turnedLeft = false;
  bool turnedRight = false;
  S32 stride = 0;

  [[nodiscard]] std::string toString() const {
    if (turnedLeft) {
      return "L";
    } else if (turnedRight) {
      return "R";
    } else {
      return std::to_string(stride);
    }
  }

  bool operator==(const RobotMovement &rhs) const {
    return turnedLeft == rhs.turnedLeft && turnedRight == rhs.turnedRight && stride == rhs.stride;
  }

  bool operator!=(const RobotMovement &rhs) const {
    return !(rhs == *this);
  }

  bool operator<(const RobotMovement &rhs) const {
    if (turnedLeft < rhs.turnedLeft) {
      return true;
    }
    if (rhs.turnedLeft < turnedLeft) {
      return false;
    }
    if (turnedRight < rhs.turnedRight) {
      return true;
    }
    if (rhs.turnedRight < turnedRight) {
      return false;
    }
    return stride < rhs.stride;
  }
};

using RobotMovementSequence = std::vector<RobotMovement>;

class Map {
public:
  std::vector<std::vector<char>> grid;
  std::optional<Pose> robot;
  RobotMovementSequence robotMovementSequence;

  void setRobot(Pose newRobot) {
    robot = newRobot;
  }

  [[nodiscard]] char getCharacter(Point<S32, 2> point) const {
    if (robot && robot->getPosition() == point) {
      return directionToArrowCharacter(robot->getDirection());
    }
    return grid[static_cast<S32>(grid.size()) - 1 - point.getY()][point.getX()];
  }

  [[nodiscard]] bool hasSupport(Point<S32, 2> point) const {
    const auto n = static_cast<S32>(grid.size());
    const auto m = static_cast<S32>(grid.front().size());
    if (point.getX() < 0 || point.getX() >= m || point.getY() < 0 || point.getY() >= n) {
      return false;
    }
    return getCharacter(point) == '#';
  }

  [[nodiscard]] bool hasIntersection(Point<S32, 2> point) const {
    const auto n = static_cast<S32>(grid.size());
    const auto m = static_cast<S32>(grid.front().size());
    if (point.getX() < 1 || point.getX() >= m - 1 || point.getY() < 1 || point.getY() >= n - 1) {
      return false;
    }
    auto result = hasSupport(point);
    result = result && hasSupport(point.move(Direction::North));
    result = result && hasSupport(point.move(Direction::East));
    result = result && hasSupport(point.move(Direction::South));
    result = result && hasSupport(point.move(Direction::West));
    return result;
  }

  [[nodiscard]] std::vector<Point<S32, 2>> getIntersections() const {
    std::vector<Point<S32, 2>> intersections;
    const auto n = static_cast<S32>(grid.size());
    const auto m = static_cast<S32>(grid.front().size());
    for (S32 y = 0; y < n; y++) {
      for (S32 x = 0; x < m; x++) {
        if (hasIntersection(Point<S32, 2>(x, y))) {
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
    if (!robotMovementSequence.empty() && !robotMovementSequence.back().turnedLeft &&
        !robotMovementSequence.back().turnedRight) {
      robotMovementSequence.back().stride++;
    } else {
      robotMovementSequence.push_back({false, false, 1});
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
    robotMovementSequence.push_back({false, true, 0});
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
    robotMovementSequence.push_back({true, false, 0});
    moveTheRobotForward();
  }

  [[nodiscard]] RobotMovementSequence getRobotMovementSequence() const {
    return robotMovementSequence;
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
      stream << map.getCharacter(Point<S32, 2>(x, y));
    }
  }
  return stream;
}

Map readMap(Intcode &intcode) {
  intcode.run();
  Pose robot;
  std::optional<Point<S32, 2>> robotOffset;
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
      robotOffset = Point<S32, 2>(x, y);
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
  const auto robotPosition = Point<S32, 2>(robotOffset->getX(), map.grid.size() - robotOffset->getY());
  robot.setPosition(robotPosition);
  map.setRobot(robot);
  return map;
}

std::string robotMovementSequenceToString(const RobotMovementSequence &sequence) {
  std::string string;
  auto first = true;
  for (const auto movement : sequence) {
    if (!first) {
      string += ",";
    }
    string += movement.toString();
    first = false;
  }
  return string;
}

S64 getRobotMovementSequenceLength(const RobotMovementSequence &sequence) {
  return robotMovementSequenceToString(sequence).size();
}

using MainRoutine = std::vector<std::variant<RobotMovement, std::size_t>>;

bool equals(const MainRoutine &main, const std::size_t &offset, const RobotMovementSequence &sequence) {
  for (std::size_t i = 0; i < sequence.size(); i++) {
    if (std::holds_alternative<RobotMovement>(main[i + offset])) {
      if (std::get<RobotMovement>(main[i + offset]) != sequence[i]) {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}

std::string mainRoutineToString(const MainRoutine &main) {
  std::string string;
  auto first = true;
  for (const auto value : main) {
    if (!first) {
      string += ",";
    }
    if (std::holds_alternative<RobotMovement>(value)) {
      string += std::get<RobotMovement>(value).toString();
    } else if (std::holds_alternative<std::size_t>(value)) {
      string += static_cast<char>('A' + std::get<std::size_t>(value));
    } else {
      throw std::invalid_argument("Somehow, the main routine has a value that is not valid.");
    }
    first = false;
  }
  return string;
}

class CompressedMovementSequence {
public:
  std::vector<std::variant<RobotMovement, std::size_t>> main;
  std::vector<RobotMovementSequence> routines;

  static S64 evaluateReplacementSavings(const RobotMovementSequence &sequence) {
    return getRobotMovementSequenceLength(sequence) - 1;
  }

  void extractRoutine() {
    std::map<RobotMovementSequence, std::size_t> counter;
    for (std::size_t i = 0; i < main.size(); i++) {
      RobotMovementSequence subsequence;
      for (std::size_t j = i; j < main.size(); j++) {
        if (!std::holds_alternative<RobotMovement>(main[j])) {
          break;
        }
        const auto movement = std::get<RobotMovement>(main[j]);
        // Only count sequences that start with a rotation.
        if (subsequence.empty()) {
          if (!movement.turnedLeft && !movement.turnedRight) {
            break;
          }
        }
        subsequence.push_back(movement);
        if (getRobotMovementSequenceLength(subsequence) > 20) {
          break;
        }
        // Do not count sequences that end with a rotation.
        if (subsequence.back().stride == 0) {
          continue;
        }
        // This adds overlapping sequences, which is not ideal.
        // ABA in ABABABA is 3, but could be 2.
        counter[subsequence]++;
      }
    }
    // Find one of the best extractions.
    RobotMovementSequence bestSequence;
    S64 bestSequenceSavings = 0;
    for (const auto &entry : counter) {
      const auto savings = static_cast<S64>(entry.second * evaluateReplacementSavings(entry.first));
      if (savings > bestSequenceSavings) {
        bestSequence = entry.first;
        bestSequenceSavings = savings;
      }
    }
    routines.push_back(bestSequence);
    for (std::size_t i = 0; i + bestSequence.size() - 1 < main.size(); i++) {
      if (equals(main, i, bestSequence)) {
        const auto index = routines.size() - 1;
        main[i] = index;
        main.erase(std::begin(main) + i + 1, std::begin(main) + i + bestSequence.size());
      }
    }
  }
};

CompressedMovementSequence compressMovementSequence(const std::vector<RobotMovement> &sequence, std::size_t routines) {
  CompressedMovementSequence compressedMovementSequence;
  for (const auto movement : sequence) {
    compressedMovementSequence.main.emplace_back(movement);
  }
  for (std::size_t i = 0; i < routines; i++) {
    compressedMovementSequence.extractRoutine();
  }
  return compressedMovementSequence;
}

void main(ArgumentParser const &argumentParser) {
  Intcode intcode(readMemory(argumentParser.getPath()));
  if (argumentParser.getPart() == 1) {
    const auto map = readMap(intcode);
    assert(!intcode.hasOutput());
    S32 total = 0;
    for (const auto intersection : map.getIntersections()) {
      total += (static_cast<S32>(map.grid.size()) - 1 - intersection.getY()) * intersection.getX();
    }
    std::cout << total << '\n';
  } else {
    intcode.memory[0] = 2;
    verify(intcode.run() == IntcodeState::Blocked);
    auto initialMap = readMap(intcode);
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
    const auto movementSequence = initialMap.getRobotMovementSequence();
    const auto compressedMovementSequence = compressMovementSequence(movementSequence, 3);
    const auto enterInputString = [&intcode](const std::string &string) {
      while (intcode.hasOutput()) {
        intcode.getOutput();
      }
      for (const auto character : string) {
        intcode.addInput(character);
      }
      intcode.addInput('\n');
      intcode.run();
    };
    const auto main = mainRoutineToString(compressedMovementSequence.main);
    enterInputString(main);
    for (std::size_t i = 0; i < 3; i++) {
      const auto routine = robotMovementSequenceToString(compressedMovementSequence.routines[i]);
      enterInputString(routine);
    }
    enterInputString("n");
    intcode.getOutput();
    while (intcode.getOutputBufferLength() > 1) {
      readMap(intcode);
    }
    std::cout << intcode.getOutput() << '\n';
  }
}
} // namespace AoC

#include "Main.inl"
