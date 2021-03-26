#include "ArgumentParser.hpp"
#include "Interval.hpp"
#include "Math.hpp"
#include "Types.hpp"

#include <cassert>

#include <iostream>
#include <string>
#include <vector>

namespace AoC {
enum ResultType { Terminated, OutOfBounds, Loop };

struct ExecutionResult {
  ResultType resultType;
  int accumulator;
};

ExecutionResult execute(std::vector<std::string> const &operations, std::vector<S32> const &arguments) {
  std::vector<bool> executed(operations.size());
  S32 accumulator = 0;
  S64 i = 0;
  auto const size = static_cast<decltype(i)>(operations.size());
  auto const interval = AoC::Interval<decltype(i)>(0, size);
  while (interval.contains(i) && !executed[unsignedCast<Unchecked>(i)]) {
    auto const uI = unsignedCast<Unchecked>(i);
    executed[uI] = true;
    if (operations[uI] == "jmp") {
      i += arguments[uI];
      continue;
    }
    if (operations[uI] == "acc") {
      accumulator += arguments[uI];
    } else {
      assert(operations[uI] == "nop");
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

void main(std::istream &stream, U32 const part) {
  std::vector<std::string> operations;
  std::vector<S32> arguments;
  {
    std::string operation;
    S32 argument;
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
