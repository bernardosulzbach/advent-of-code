#include "Robot.hpp"

Robot::Robot(Direction initialDirection) : direction(initialDirection) {
}

void Robot::rotateRight(RobotCoordinateType angle) {
  static_assert(std::is_signed_v<RobotCoordinateType>, "Needs to be able to use negative angles.");
  assert(angle % 90 == 0);
  auto const counterClockwise = angle < 0;
  for (auto rotations = std::abs(angle / 90) % 4; rotations > 0; rotations--) {
    if (counterClockwise) {
      direction = rotateCounterClockwise(direction);
    } else {
      direction = rotateClockwise(direction);
    }
  }
}

void Robot::rotateLeft(RobotCoordinateType angle) {
  static_assert(std::is_signed_v<RobotCoordinateType>, "Needs to be able to use negative angles.");
  rotateRight(-angle);
}

void Robot::goForward(RobotCoordinateType const amount) {
  p = p.move(direction, amount);
}

void Robot::goInDirection(Direction const movementDirection, RobotCoordinateType const amount) {
  p = p.move(movementDirection, amount);
}

Point<RobotCoordinateType> Robot::getPosition() const {
  return p;
}

void Robot::setPosition(Point<RobotCoordinateType> const newPosition) {
  p = newPosition;
}

Direction Robot::getDirection() const {
  return direction;
}

void Robot::setDirection(Direction const newDirection) {
  direction = newDirection;
}
