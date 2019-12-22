#pragma once

#include "Direction.hpp"
#include "Point.hpp"
#include "Types.hpp"

class Robot {
  Point<S32> p{0, 0};

  Direction direction = Direction::North;

public:
  void rotateRight();

  void rotateLeft();

  void goForward();

  [[nodiscard]] Point<S32> getPosition() const;

  void setPosition(Point<S32> newPosition);

  [[nodiscard]] Direction getDirection() const;

  void setDirection(Direction newDirection);
};
