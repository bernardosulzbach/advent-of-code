#include "Filesystem.hpp"

#include <fstream>

#include <boost/algorithm/string.hpp>

std::string readFile(const std::filesystem::path &path) {
  std::ostringstream buffer;
  std::ifstream input(path.native());
  buffer << input.rdbuf();
  auto asString = buffer.str();
  boost::trim(asString);
  return asString;
}
