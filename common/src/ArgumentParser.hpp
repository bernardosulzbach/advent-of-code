#pragma once

#include "Types.hpp"

#include <optional>
#include <string>
#include <vector>

namespace AoC {
class ArgumentParser {
  using AdditionalArgumentType = std::string;

  std::string path;
  U32 part;
  std::vector<AdditionalArgumentType> additionalArguments;

public:
  void parseArguments(int argc, char **argv);

  [[nodiscard]] std::string getPath() const noexcept;

  [[nodiscard]] U32 getPart() const noexcept;

  [[nodiscard]] std::string getAdditionalArgument(Size index) const;
};
} // namespace AoC