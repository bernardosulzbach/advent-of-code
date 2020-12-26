#pragma once

#include "Direction.hpp"
#include "Point.hpp"
#include "Types.hpp"

using RobotCoordinateType = S32;

/**
 * This class represents a robot with position and orientation on a plane.
 */
class Robot {
  Point<RobotCoordinateType> p{0, 0};

  Direction direction = Direction::North;

public:
  Robot() = default;

  explicit Robot(Direction initialDirection);

  void rotateRight(RobotCoordinateType angle = 90);

  void rotateLeft(RobotCoordinateType angle = 90);

  void goForward(RobotCoordinateType amount = 1);

  void goInDirection(Direction movementDirection, RobotCoordinateType amount);

  [[nodiscard]] Point<RobotCoordinateType> getPosition() const;

  void setPosition(Point<RobotCoordinateType> newPosition);

  [[nodiscard]] Direction getDirection() const;

  void setDirection(Direction newDirection);
};
