#pragma once

#include "Types.hpp"

#include <optional>
#include <string>
#include <vector>

class ArgumentParser {
  using AdditionalArgumentType = S64;

  std::optional<std::string> path;
  std::vector<AdditionalArgumentType> additionalArguments;

public:
  void parseArguments(int argc, char **argv);

  [[nodiscard]] std::string getPath() const;

  [[nodiscard]] AdditionalArgumentType getAdditionalArgument(Size index) const;
};
