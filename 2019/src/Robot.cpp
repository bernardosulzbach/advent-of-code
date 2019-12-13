#include "Robot.hpp"

void Robot::rotateRight() {
  direction = rotateClockwise(direction);
}

void Robot::rotateLeft() {
  direction = rotateCounterClockwise(direction);
}

void Robot::goForward() {
  switch (direction) {
  case Direction::North:
    y++;
    return;
  case Direction::East:
    x++;
    return;
  case Direction::South:
    y--;
    return;
  case Direction::West:
    x--;
    return;
  }
  throw std::runtime_error("Should never happen.");
}

Point<S32> Robot::getPosition() {
  return {x, y};
}
