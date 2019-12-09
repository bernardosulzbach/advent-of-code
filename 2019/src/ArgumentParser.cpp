#include "ArgumentParser.hpp"

#include <stdexcept>

void ArgumentParser::parseArguments(int argc, char **argv) {
  if (argc >= 2) {
    path = std::string(argv[1]);
  }
  for (int i = 2; i < argc; i++) {
    additionalArguments.push_back(std::stoi(argv[i]));
  }
}

std::string ArgumentParser::getPath() {
  if (path) {
    return path.value();
  }
  throw std::runtime_error("Does not have a path.");
}

int ArgumentParser::getAdditionalArgument(int index) const {
  return additionalArguments.at(index);
}
