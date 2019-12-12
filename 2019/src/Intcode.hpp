#pragma once

#include <array>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "IntcodeState.hpp"
#include "Types.hpp"

class Intcode {
public:
  using ValueType = S64;
  using IndexType = U32;

private:
  enum class Opcode {
    Add = 1,
    Multiply = 2,
    Input = 3,
    Output = 4,
    JumpIfTrue = 5,
    JumpIfFalse = 6,
    LessThan = 7,
    Equals = 8,
    AdjustRelativeBase = 9,
    HaltInstruction = 99
  };

  constexpr static std::array<Opcode, 10> opcodes = {
      Opcode::Add,         Opcode::Multiply, Opcode::Input,  Opcode::Output,          Opcode::JumpIfTrue,
      Opcode::JumpIfFalse, Opcode::LessThan, Opcode::Equals, Opcode::HaltInstruction, Opcode::AdjustRelativeBase};

  IndexType instructionPointer = 0;
  IndexType relativeBase = 0;

  std::deque<ValueType> inputBuffer;
  std::deque<ValueType> outputBuffer;

  bool debugging = false;

  void printMemory() const;

  void writeMessageIfDebugging(const std::string &message) const;

public:
  std::vector<ValueType> memory;

  explicit Intcode(std::vector<ValueType> initialMemory) : memory(std::move(initialMemory)) {
  }

  void setDebugging(bool value);

  [[nodiscard]] Opcode getInstructionType() const;

  [[nodiscard]] ValueType &readMemory(IndexType index);

  ValueType &getOperand(IndexType operandIndex);

  void addInput(ValueType input);

  [[nodiscard]] bool hasOutput() const;

  ValueType getOutput();

  void setInstructionPointer(std::size_t newInstructionPointer);

  IntcodeState run();
};

std::vector<Intcode::ValueType> readMemory(const std::string &path);