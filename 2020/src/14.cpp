#include "ArgumentParser.hpp"
#include "BitMask.hpp"
#include "IO.hpp"
#include "Pose.hpp"
#include "Ranges.hpp"
#include "Types.hpp"

#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
    using AddressType = U64;
    using ValueType = U64;
    std::unordered_map<AddressType, ValueType> memory;
    AoC::BitMask bitMask;
    std::string lhs;
    while (stream >> lhs) {
      stream.ignore(3);
      auto rhs = AoC::readValue<std::string>(stream);
      if (lhs.starts_with("mask")) {
        if (part == 1) {
          std::ranges::replace(rhs, 'X', '-');
        } else {
          std::ranges::replace(rhs, '0', '-');
          std::ranges::replace(rhs, 'X', '*');
        }
        bitMask = AoC::BitMask::bitMaskFromString(rhs);
      } else {
        auto const startOfAddress = lhs.find('[') + 1;
        assert(startOfAddress != std::string::npos);
        auto const address = AoC::parseValue<AddressType>(lhs.substr(startOfAddress, lhs.size() - 1 - startOfAddress));
        auto const value = AoC::parseValue<ValueType>(rhs);
        if (part == 1) {
          memory[address] = AoC::only(bitMask.apply(value));
        } else {
          for (auto const maskedAddress : bitMask.apply(address)) {
            memory[maskedAddress] = value;
          }
        }
      }
    }
    ValueType sum{};
    std::ranges::for_each(memory, [&sum](auto const &entry) { sum += entry.second; });
    std::cout << sum << '\n';
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
