#pragma once

#include <cmath>
#include <stdexcept>

#include "Types.hpp"

class Vector {
public:
  F64 x = 0.0;
  F64 y = 0.0;

  Vector(F64 vX, F64 vY);

  [[nodiscard]] Vector subtract(const Vector &rhs) const;

  [[nodiscard]] F64 getNorm() const;

  [[nodiscard]] Vector normalize() const;

  [[nodiscard]] F64 getAngle() const;
};
