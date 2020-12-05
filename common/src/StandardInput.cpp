#include "StandardInput.hpp"

#include <iostream>

int readInt() {
  int x;
  if (std::cin >> x) {
    return x;
  }
  throw std::runtime_error("Should not fail to read from standard input.");
}