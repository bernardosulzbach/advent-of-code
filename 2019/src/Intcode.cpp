#include "Intcode.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

void Intcode::printMemory() const {
  for (std::size_t i = 0; i < memory.size(); i++) {
    if (i > 0) {
      std::cout << ", ";
    }
    std::cout << memory[i];
  }
  std::cout << '\n';
}

void Intcode::setDebugging(bool value) {
  debugging = value;
}

void Intcode::run() {
  if (debugging) {
    printMemory();
  }
  instructionPointer = 0;
  while (instructionPointer < memory.size()) {
    const auto operationCode = memory[instructionPointer];
    if (operationCode == Intcode::AddInstruction) {
      const auto &a = memory[memory[instructionPointer + 1]];
      const auto &b = memory[memory[instructionPointer + 2]];
      auto &c = memory[memory[instructionPointer + 3]];
      c = a + b;
      instructionPointer += 4;
      if (debugging) {
        std::cout << "Added." << '\n';
      }
    } else if (operationCode == Intcode::MultiplyInstruction) {
      const auto &a = memory[memory[instructionPointer + 1]];
      const auto &b = memory[memory[instructionPointer + 2]];
      auto &c = memory[memory[instructionPointer + 3]];
      c = a * b;
      instructionPointer += 4;
      if (debugging) {
        std::cout << "Multiplied." << '\n';
      }
    } else if (operationCode == Intcode::HaltInstruction) {
      if (debugging) {
        std::cout << "Halted." << '\n';
      }
      break;
    } else {
      throw std::invalid_argument("Invalid instruction: " + std::to_string(operationCode) + ".");
    }
  }
  if (debugging) {
    printMemory();
  }
}

std::vector<int> readMemory(const std::string &path) {
  std::ifstream stream(path);
  std::vector<int> memory;
  int value;
  while (stream >> value) {
    memory.push_back(value);
    char comma;
    stream >> comma;
  }
  return memory;
}
