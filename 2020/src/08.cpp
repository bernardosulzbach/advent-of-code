#include "ArgumentParser.hpp"
#include "Math.hpp"
#include "Types.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
  auto const interval = Interval<decltype(i)>(0, size - 1);
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

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
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
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
