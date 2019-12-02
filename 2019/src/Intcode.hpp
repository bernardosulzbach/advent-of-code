#pragma once

#include <string>
#include <utility>
#include <vector>

class Intcode {
  constexpr static int AddInstruction = 1;
  constexpr static int MultiplyInstruction = 2;
  constexpr static int HaltInstruction = 99;

  std::size_t instructionPointer = 0;

  bool debugging = false;

  void printMemory() const;

public:
  std::vector<int> memory;

  explicit Intcode(std::vector<int> initialMemory) : memory(std::move(initialMemory)) {
  }

  void setDebugging(bool value);

  void run();
};

std::vector<int> readMemory(const std::string &path);