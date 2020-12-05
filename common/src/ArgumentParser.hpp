#pragma once

#include <optional>
#include <string>
#include <vector>

class ArgumentParser {
  std::optional<std::string> path;
  std::vector<int> additionalArguments;

public:
  void parseArguments(int argc, char **argv);

  std::string getPath();

  int getAdditionalArgument(int index) const;
};