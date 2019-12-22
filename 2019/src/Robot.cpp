#include "Robot.hpp"

void Robot::rotateRight() {
  direction = rotateClockwise(direction);
}

void Robot::rotateLeft() {
  direction = rotateCounterClockwise(direction);
}

void Robot::goForward() {
  p = p.move(direction);
}

Point<S32> Robot::getPosition() const {
  return p;
}

void Robot::setPosition(Point<S32> newPosition) {
  p = newPosition;
}

Direction Robot::getDirection() const {
  return direction;
}

void Robot::setDirection(Direction newDirection) {
  direction = newDirection;
}
