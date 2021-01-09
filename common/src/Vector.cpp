#include "Vector.hpp"

F64 getAngle(Vector<F64, 2> const vector) noexcept {
  return std::atan2(vector.components[1], vector.components[0]);
}
