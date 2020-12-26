#include "Pose.hpp"

Pose::Pose(Direction initialDirection) : direction(initialDirection) {
}

void Pose::rotateRight(RobotCoordinateType angle) {
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

void Pose::rotateLeft(RobotCoordinateType angle) {
  static_assert(std::is_signed_v<RobotCoordinateType>, "Needs to be able to use negative angles.");
  rotateRight(-angle);
}

void Pose::goForward(RobotCoordinateType const amount) {
  p = p.move(direction, amount);
}

void Pose::goInDirection(Direction const movementDirection, RobotCoordinateType const amount) {
  p = p.move(movementDirection, amount);
}

Point<RobotCoordinateType> Pose::getPosition() const {
  return p;
}

void Pose::setPosition(Point<RobotCoordinateType> const newPosition) {
  p = newPosition;
}

Direction Pose::getDirection() const {
  return direction;
}

void Pose::setDirection(Direction const newDirection) {
  direction = newDirection;
}
