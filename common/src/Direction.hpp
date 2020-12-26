#pragma once

enum class Direction { North, East, South, West, Up = North, Right = East, Down = South, Left = West };

Direction rotateClockwise(Direction direction);

Direction rotateCounterClockwise(Direction direction);

char directionToCharacter(Direction direction);

Direction characterToDirection(char character);

char directionToArrowCharacter(Direction direction);