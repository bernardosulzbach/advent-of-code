#include "Filesystem.hpp"

#include <cassert>
#include <sstream>

#include <boost/algorithm/string.hpp>

std::string readFile(std::istream const &stream) {
  assert(stream);
  std::ostringstream buffer;
  buffer << stream.rdbuf();
  auto asString = buffer.str();
  boost::trim(asString);
  return asString;
}
