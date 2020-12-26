#pragma once

#include "Direction.hpp"
#include "Point.hpp"
#include "Types.hpp"

using RobotCoordinateType = S32;

/**
 * This class represents a pose (the position of an object on a plane and the direction it's facing (its orientation)).
 */
class Pose {
  Point<RobotCoordinateType> p{0, 0};

  Direction direction = Direction::North;

public:
  Pose() = default;

  explicit Pose(Direction initialDirection);

  void rotateRight(RobotCoordinateType angle = 90);

  void rotateLeft(RobotCoordinateType angle = 90);

  void goForward(RobotCoordinateType amount = 1);

  void goInDirection(Direction movementDirection, RobotCoordinateType amount);

  [[nodiscard]] Point<RobotCoordinateType> getPosition() const;

  void setPosition(Point<RobotCoordinateType> newPosition);

  [[nodiscard]] Direction getDirection() const;

  void setDirection(Direction newDirection);
};
