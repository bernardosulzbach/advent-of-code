#pragma once

enum class Direction { North, East, South, West };

Direction rotateClockwise(Direction direction);

Direction rotateCounterClockwise(Direction direction);

char directionToCharacter(Direction direction);

char directionToArrowCharacter(Direction direction);