#include "Vector.hpp"

Vector::Vector(F64 vX, F64 vY) : x(vX), y(vY) {
}

Vector Vector::subtract(const Vector &rhs) const {
  return Vector(x - rhs.x, y - rhs.y);
}

F64 Vector::getNorm() const {
  return hypot(x, y);
}

Vector Vector::normalize() const {
  double norm = getNorm();
  if (norm == 0.0) {
    throw std::invalid_argument("Null vector cannot be normalized.");
  }
  return Vector(x / norm, y / norm);
}

F64 Vector::getAngle() const {
  return std::atan2(y, x);
}
