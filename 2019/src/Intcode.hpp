#pragma once

#include <array>
#include <stack>
#include <string>
#include <utility>
#include <vector>

class Intcode {
  enum class Opcode {
    Add = 1,
    Multiply = 2,
    Input = 3,
    Output = 4,
    JumpIfTrue = 5,
    JumpIfFalse = 6,
    LessThan = 7,
    Equals = 8,
    HaltInstruction = 99
  };

  constexpr static std::array<Opcode, 9> opcodes = {Opcode::Add,      Opcode::Multiply,   Opcode::Input,
                                                    Opcode::Output,   Opcode::JumpIfTrue, Opcode::JumpIfFalse,
                                                    Opcode::LessThan, Opcode::Equals,     Opcode::HaltInstruction};

  std::size_t instructionPointer = 0;

  std::deque<int> userInputBuffer;

  bool debugging = false;

  void printMemory() const;

  void writeMessageIfDebugging(const std::string &message) const;

public:
  std::vector<int> memory;

  explicit Intcode(std::vector<int> initialMemory) : memory(std::move(initialMemory)) {
  }

  void setDebugging(bool value);

  [[nodiscard]] Opcode getInstructionType() const;

  int &getOperand(int operandIndex);

  void addUserInput(int input);

  void run();
};

std::vector<int> readMemory(const std::string &path);