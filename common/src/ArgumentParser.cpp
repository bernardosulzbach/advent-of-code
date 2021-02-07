#include "ArgumentParser.hpp"

#include "IO.hpp"
#include "Interval.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace AoC {
void ArgumentParser::parseArguments(int argc, char **argv) {
  if (argc < 2) {
    throw std::runtime_error("Expected a path as the first argument.");
  }
  path = std::string(argv[1]);
  if (argc < 3) {
    throw std::runtime_error("Expected a part as the second argument.");
  }
  part = parseValue<decltype(part)>(std::string(argv[2]));
  if (!Interval<decltype(part)>(1, 3).contains(part)) {
    throw std::runtime_error("Part should be 1 or 2.");
  }
  for (decltype(argc) i = 3; i < argc; i++) {
    ArgumentParser::AdditionalArgumentType argument;
    std::stringstream stream(argv[i]);
    stream >> argument;
    additionalArguments.push_back(argument);
  }
}

std::string ArgumentParser::getPath() const noexcept {
  return path;
}

U32 ArgumentParser::getPart() const noexcept {
  return part;
}

ArgumentParser::AdditionalArgumentType ArgumentParser::getAdditionalArgument(Size const index) const {
  if (index >= additionalArguments.size()) {
    std::cerr << "There are not enough arguments." << '\n';
  }
  return additionalArguments.at(index);
}
} // namespace AoC
