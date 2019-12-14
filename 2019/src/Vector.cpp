#include "Vector.hpp"

F64 getAngle(const Vector<F64, 2> vector) {
  return std::atan2(vector.components[1], vector.components[0]);
}
