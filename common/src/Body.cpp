#include "Body.hpp"

#include <fstream>

std::istream &operator>>(std::istream &stream, Body &body) {
  for (auto &component : body.position.components) {
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '=');
    stream >> component;
  }
  stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const Body &body) {
  stream << "<x=" << body.position.components[0];
  stream << ", y=" << body.position.components[1];
  stream << ", z=" << body.position.components[2];
  stream << ">";
  return stream;
}

std::vector<Body> readBodies(std::istream &stream) {
  std::vector<Body> bodies;
  Body body{};
  while (stream >> body) {
    bodies.push_back(body);
  }
  return bodies;
}

S64 Body::getEnergy() const {
  return position.getL1Norm() + velocity.getL1Norm();
}
