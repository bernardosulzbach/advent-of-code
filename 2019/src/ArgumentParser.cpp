#include "ArgumentParser.hpp"

#include <stdexcept>

std::string parseArguments(int argc, char **argv) {
  if (argc != 2) {
    throw std::invalid_argument("Argument count should be 2.");
  }
  return std::string(argv[1]);
}