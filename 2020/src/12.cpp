#include "ArgumentParser.hpp"
#include "Direction.hpp"
#include "IO.hpp"
#include "Pose.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
    auto const lines = AoC::readVector<std::string>(stream);
    auto ship = Pose(Direction::East);
    if (part == 1) {
      for (auto const &line : lines) {
        std::stringstream lineStream(line);
        char command;
        int amount;
        lineStream >> command >> amount;
        if (command == 'N' || command == 'E' || command == 'S' || command == 'W') {
          ship.goInDirection(characterToDirection(command), amount);
        } else if (command == 'L') {
          ship.rotateLeft(amount);
        } else if (command == 'R') {
          ship.rotateRight(amount);
        } else if (command == 'F') {
          ship.goForward(amount);
        }
      }
    } else if (part == 2) {
      auto waypoint = Pose();
      waypoint.goInDirection(Direction::East, 10);
      waypoint.goInDirection(Direction::North, 1);
      for (auto const &line : lines) {
        std::stringstream lineStream(line);
        char command;
        int amount;
        lineStream >> command >> amount;
        if (command == 'N' || command == 'E' || command == 'S' || command == 'W') {
          waypoint.goInDirection(characterToDirection(command), amount);
        } else if (command == 'L') {
          waypoint.setPosition(waypoint.getPosition().rotateAround(Point<S32>(), amount));
        } else if (command == 'R') {
          waypoint.setPosition(waypoint.getPosition().rotateAround(Point<S32>(), -amount));
        } else if (command == 'F') {
          ship.setPosition(ship.getPosition().shift(waypoint.getPosition().subtract(Point<S32>()).scale(amount)));
        }
      }
    }
    std::cout << ship.getPosition().subtract(Point<S32>{}).getL1Norm() << '\n';
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
