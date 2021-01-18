#include "ArgumentParser.hpp"
#include "Interval.hpp"
#include "Math.hpp"
#include "Types.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace AoC {
enum ResultType { Terminated, OutOfBounds, Loop };

struct ExecutionResult {
  ResultType resultType;
  int accumulator;
};

ExecutionResult execute(std::vector<std::string> const &operations, std::vector<int> const &arguments) {
  std::vector<bool> executed(operations.size());
  int accumulator = 0;
  S64 i = 0;
  auto const size = static_cast<decltype(i)>(operations.size());
  auto const interval = AoC::Interval<decltype(i)>(0, size);
  while (interval.contains(i) && !executed[i]) {
    executed[i] = true;
    if (operations[i] == "jmp") {
      i += arguments[i];
      continue;
    }
    if (operations[i] == "acc") {
      accumulator += arguments[i];
    } else {
      assert(operations[i] == "nop");
    }
    ++i;
  }
  if (interval.contains(i)) {
    return ExecutionResult{ResultType::Loop, accumulator};
  }
  if (i == size) {
    return ExecutionResult{ResultType::Terminated, accumulator};
  }
  return ExecutionResult{ResultType::OutOfBounds, accumulator};
}

void main(ArgumentParser const &argumentParser) {
  auto const part = argumentParser.getPart();
  auto stream = std::ifstream(argumentParser.getPath());
  std::vector<std::string> operations;
  std::vector<int> arguments;
  {
    std::string operation;
    int argument;
    while (stream >> operation >> argument) {
      operations.push_back(operation);
      arguments.push_back(argument);
    }
  }
  if (part == 1) {
    auto const executionResult = execute(operations, arguments);
    std::cout << executionResult.accumulator << '\n';
  } else {
    for (auto it = begin(operations); it != end(operations); ++it) {
      if (*it == "acc") {
        continue;
      }
      std::string replacement = *it == "nop" ? "jmp" : "nop";
      it->swap(replacement);
      if (auto const result = execute(operations, arguments); result.resultType == ResultType::Terminated) {
        std::cout << result.accumulator << '\n';
        break;
      }
      it->swap(replacement);
    }
  }
}
} // namespace AoC

#include "Main.inl"
