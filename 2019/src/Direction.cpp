#include "Direction.hpp"

#include <stdexcept>

Direction rotateClockwise(Direction direction) {
  switch (direction) {
  case Direction::North:
    return Direction::East;
  case Direction::East:
    return Direction::South;
  case Direction::South:
    return Direction::West;
  case Direction::West:
    return Direction::North;
  }
  throw std::runtime_error("Should never happen.");
}

Direction rotateCounterClockwise(Direction direction) {
  return rotateClockwise(rotateClockwise(rotateClockwise(direction)));
}
