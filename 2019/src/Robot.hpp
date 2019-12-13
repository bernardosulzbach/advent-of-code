#pragma once

#include "Direction.hpp"
#include "Point.hpp"
#include "Types.hpp"

class Robot {
  S32 x = 0;
  S32 y = 0;

  Direction direction = Direction::North;

public:
  void rotateRight();

  void rotateLeft();

  void goForward();

  Point<S32> getPosition();
};
