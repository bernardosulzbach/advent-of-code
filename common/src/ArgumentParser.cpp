#include "ArgumentParser.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

void ArgumentParser::parseArguments(int argc, char **argv) {
  if (argc >= 2) {
    path = std::string(argv[1]);
  }
  for (int i = 2; i < argc; i++) {
    ArgumentParser::AdditionalArgumentType argument;
    std::stringstream stream(argv[i]);
    stream >> argument;
    additionalArguments.push_back(argument);
  }
}

std::string ArgumentParser::getPath() const {
  if (path) {
    return path.value();
  }
  throw std::runtime_error("Does not have a path.");
}

ArgumentParser::AdditionalArgumentType ArgumentParser::getAdditionalArgument(Size index) const {
  if (index >= additionalArguments.size()) {
    std::cerr << "There are not enough arguments." << '\n';
  }
  return additionalArguments.at(index);
}
