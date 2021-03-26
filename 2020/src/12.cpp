#include "ArgumentParser.hpp"
#include "Direction.hpp"
#include "IO.hpp"
#include "Pose.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

#include <iostream>
#include <vector>

namespace AoC {
void main(std::istream &stream, U32 const part) {
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
        waypoint.setPosition(waypoint.getPosition().rotateAround(Point<S32, 2>(), amount));
      } else if (command == 'R') {
        waypoint.setPosition(waypoint.getPosition().rotateAround(Point<S32, 2>(), -amount));
      } else if (command == 'F') {
        ship.setPosition(ship.getPosition() + Point<S32, 2>{}.to(waypoint.getPosition()).scale(amount));
      }
    }
  }
  std::cout << ship.getPosition().toOrigin().getL1Norm() << '\n';
}
} // namespace AoC

#include "Main.inl"
