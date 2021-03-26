#include "Intcode.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "IO.hpp"
#include "IntcodeState.hpp"
#include "Math.hpp"

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
  const auto instructionOpcode = memory[AoC::checkedCast<Size>(instructionPointer)] % 100;
  for (const auto opcode : opcodes) {
    if (static_cast<int>(opcode) == instructionOpcode) {
      return opcode;
    }
  }
  throw std::invalid_argument("Invalid opcode: " + std::to_string(instructionOpcode) + ".");
}

void Intcode::resizeMemoryIfNeeded(IndexType index) {
  if (index < 0) {
    throw std::invalid_argument("Cannot use an index < 0 for memory access.");
  }
  const auto oldSize = memory.size();
  if (index >= static_cast<S64>(memory.size())) {
    memory.resize(AoC::checkedCast<Size>(index + 1));
  } else {
    return;
  }
  const auto newSize = memory.size();
  if (debugging) {
    std::cout << "Resized memory from " << oldSize << " to " << newSize << "." << '\n';
  }
}

Intcode::ValueType Intcode::readMemory(IndexType index) {
  resizeMemoryIfNeeded(index);
  return memory[AoC::checkedCast<Size>(index)];
}

void Intcode::writeMemory(IndexType index, ValueType value) {
  resizeMemoryIfNeeded(index);
  memory[AoC::checkedCast<Size>(index)] = value;
}

Intcode::IndexType Intcode::getMemoryIndexOfOperand(IndexType operandIndex) {
  auto mode = memory.at(AoC::checkedCast<Size>(instructionPointer)) / 100;
  for (auto i = operandIndex; i > 1; i--) {
    mode /= 10;
  }
  mode %= 10;
  if (mode == 0) {
    return readMemory(instructionPointer + operandIndex);
  } else if (mode == 1) {
    return instructionPointer + operandIndex;
  } else if (mode == 2) {
    return relativeBase + readMemory(instructionPointer + operandIndex);
  }
  throw std::invalid_argument("Unrecognized operator mode.");
}

Intcode::ValueType Intcode::readFromOperand(IndexType operandIndex) {
  return readMemory(getMemoryIndexOfOperand(operandIndex));
}

void Intcode::writeIntoOperand(IndexType operandIndex, ValueType value) {
  writeMemory(getMemoryIndexOfOperand(operandIndex), value);
}

void Intcode::writeMessageIfDebugging(const std::string &message) const {
  if (debugging) {
    std::cout << message << '\n';
  }
}

void Intcode::addInput(Intcode::ValueType input) {
  inputBuffer.push_back(input);
}

std::size_t Intcode::getOutputBufferLength() const {
  return outputBuffer.size();
}

bool Intcode::hasOutput() const {
  return !outputBuffer.empty();
}

Intcode::ValueType Intcode::getOutput() {
  if (outputBuffer.empty()) {
    throw std::runtime_error("Attempted to read from an empty buffer.");
  }
  const auto result = outputBuffer.front();
  outputBuffer.pop_front();
  return result;
}

void Intcode::setInstructionPointer(IndexType const newInstructionPointer) noexcept {
  instructionPointer = newInstructionPointer;
}

IntcodeState Intcode::run() {
  if (debugging) {
    printMemory();
  }
  while (true) {
    if (debugging) {
      std::cout << "Instruction pointer is " << instructionPointer << "." << '\n';
    }
    if (getInstructionType() == Opcode::Add) {
      writeIntoOperand(3, readFromOperand(1) + readFromOperand(2));
      instructionPointer += 4;
      writeMessageIfDebugging("Added.");
    } else if (getInstructionType() == Opcode::Multiply) {
      writeIntoOperand(3, readFromOperand(1) * readFromOperand(2));
      instructionPointer += 4;
      writeMessageIfDebugging("Multiplied.");
    } else if (getInstructionType() == Opcode::Input) {
      if (inputBuffer.empty()) {
        return IntcodeState::Blocked;
      } else {
        writeIntoOperand(1, inputBuffer.front());
        inputBuffer.pop_front();
      }
      instructionPointer += 2;
      writeMessageIfDebugging("Read input.");
    } else if (getInstructionType() == Opcode::Output) {
      outputBuffer.push_back(readFromOperand(1));
      instructionPointer += 2;
      writeMessageIfDebugging("Wrote output.");
    } else if (getInstructionType() == Opcode::JumpIfTrue) {
      if (readFromOperand(1) != 0) {
        instructionPointer = readFromOperand(2);
      } else {
        instructionPointer += 3;
      }
      writeMessageIfDebugging("Evaluated jump if true.");
    } else if (getInstructionType() == Opcode::JumpIfFalse) {
      if (readFromOperand(1) == 0) {
        instructionPointer = readFromOperand(2);
      } else {
        instructionPointer += 3;
      }
      writeMessageIfDebugging("Evaluated jump if false.");
    } else if (getInstructionType() == Opcode::LessThan) {
      const auto &a = readFromOperand(1);
      const auto &b = readFromOperand(2);
      writeIntoOperand(3, a < b ? 1 : 0);
      instructionPointer += 4;
      writeMessageIfDebugging("Tested if less than.");
    } else if (getInstructionType() == Opcode::Equals) {
      const auto &a = readFromOperand(1);
      const auto &b = readFromOperand(2);
      writeIntoOperand(3, a == b ? 1 : 0);
      instructionPointer += 4;
      writeMessageIfDebugging("Tested for equality.");
    } else if (getInstructionType() == Opcode::AdjustRelativeBase) {
      const auto oldRelativeBase = std::to_string(relativeBase);
      relativeBase += readFromOperand(1);
      const auto newRelativeBase = std::to_string(relativeBase);
      instructionPointer += 2;
      const auto oldToNew = oldRelativeBase + " to " + newRelativeBase;
      writeMessageIfDebugging("Adjusted the relative base from " + oldToNew + ".");
    } else if (getInstructionType() == Opcode::HaltInstruction) {
      if (debugging) {
        writeMessageIfDebugging("Halted.");
      }
      if (debugging) {
        printMemory();
      }
      return IntcodeState::Halted;
    }
  }
  throw std::runtime_error("Should never happen.");
}

std::vector<Intcode::ValueType> readMemory(std::istream &stream) {
  std::vector<Intcode::ValueType> memory;
  Intcode::ValueType value;
  while (stream >> value) {
    memory.push_back(value);
    char comma;
    stream >> comma;
  }
  return memory;
}
