#pragma once

#include "Vector.hpp"

#include <boost/functional/hash/hash.hpp>

#include <fstream>
#include <vector>

class Body {
public:
  Vector<S64, 3> position;
  Vector<S64, 3> velocity;

  [[nodiscard]] S64 getEnergy() const;
};

std::istream &operator>>(std::istream &stream, Body &body);

std::ostream &operator<<(std::ostream &stream, const Body &body);

std::vector<Body> readBodies(std::istream &stream);

template <> struct std::hash<Body> {
  std::size_t operator()(const Body &body) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, std::hash<Vector<S64, 3>>()(body.position));
    boost::hash_combine(seed, std::hash<Vector<S64, 3>>()(body.velocity));
    return seed;
  }
};
