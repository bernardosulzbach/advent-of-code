#include "Intcode.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "StandardInput.hpp"

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

Intcode::Opcode Intcode::getInstructionType() const {
  const auto instructionOpcode = memory[instructionPointer] % 100;
  for (const auto opcode : opcodes) {
    if (static_cast<int>(opcode) == instructionOpcode) {
      return opcode;
    }
  }
  throw std::invalid_argument("Invalid opcode: " + std::to_string(instructionOpcode) + ".");
}

int &Intcode::getOperand(int operandIndex) {
  auto mode = memory[instructionPointer] / 100;
  for (int i = operandIndex; i > 1; i--) {
    mode /= 10;
  }
  mode %= 10;
  if (mode == 0) {
    return memory[memory[instructionPointer + operandIndex]];
  } else if (mode == 1) {
    return memory[instructionPointer + operandIndex];
  }
  throw std::invalid_argument("Unrecognized operator mode.");
}

void Intcode::writeMessageIfDebugging(const std::string &message) const {
  if (debugging) {
    std::cout << message << '\n';
  }
}

void Intcode::addUserInput(int input) {
  userInputBuffer.push_back(input);
}

void Intcode::run() {
  if (debugging) {
    printMemory();
  }
  instructionPointer = 0;
  while (instructionPointer < memory.size()) {
    if (getInstructionType() == Opcode::Add) {
      const auto &a = getOperand(1);
      const auto &b = getOperand(2);
      auto &c = getOperand(3);
      c = a + b;
      instructionPointer += 4;
      writeMessageIfDebugging("Added.");
    } else if (getInstructionType() == Opcode::Multiply) {
      const auto &a = getOperand(1);
      const auto &b = getOperand(2);
      auto &c = getOperand(3);
      c = a * b;
      instructionPointer += 4;
      writeMessageIfDebugging("Multiplied.");
    } else if (getInstructionType() == Opcode::Input) {
      auto &a = getOperand(1);
      if (userInputBuffer.empty()) {
        a = readInt();
      } else {
        a = userInputBuffer.front();
        userInputBuffer.pop_front();
      }
      instructionPointer += 2;
      writeMessageIfDebugging("Read input.");
    } else if (getInstructionType() == Opcode::Output) {
      std::cout << getOperand(1) << '\n';
      instructionPointer += 2;
      writeMessageIfDebugging("Wrote output.");
    } else if (getInstructionType() == Opcode::JumpIfTrue) {
      if (getOperand(1) != 0) {
        instructionPointer = getOperand(2);
      } else {
        instructionPointer += 3;
      }
      writeMessageIfDebugging("Evaluated jump if true.");
    } else if (getInstructionType() == Opcode::JumpIfFalse) {
      if (getOperand(1) == 0) {
        instructionPointer = getOperand(2);
      } else {
        instructionPointer += 3;
      }
      writeMessageIfDebugging("Evaluated jump if false.");
    } else if (getInstructionType() == Opcode::LessThan) {
      const auto &a = getOperand(1);
      const auto &b = getOperand(2);
      auto &c = getOperand(3);
      c = a < b ? 1 : 0;
      instructionPointer += 4;
      writeMessageIfDebugging("Tested if less than.");
    } else if (getInstructionType() == Opcode::Equals) {
      const auto &a = getOperand(1);
      const auto &b = getOperand(2);
      auto &c = getOperand(3);
      c = a == b ? 1 : 0;
      instructionPointer += 4;
      writeMessageIfDebugging("Tested for equality.");
    } else if (getInstructionType() == Opcode::HaltInstruction) {
      if (debugging) {
        writeMessageIfDebugging("Halted.");
      }
      break;
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
