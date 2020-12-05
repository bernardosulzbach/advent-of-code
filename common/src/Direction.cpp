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

char directionToCharacter(Direction direction) {
  switch (direction) {
  case Direction::North:
    return 'N';
  case Direction::East:
    return 'E';
  case Direction::South:
    return 'S';
  case Direction::West:
    return 'W';
  }
  throw std::runtime_error("Should never happen.");
}

char directionToArrowCharacter(Direction direction) {
  switch (direction) {
  case Direction::North:
    return '^';
  case Direction::East:
    return '>';
  case Direction::South:
    return 'v';
  case Direction::West:
    return '<';
  }
  throw std::runtime_error("Should never happen.");
}
