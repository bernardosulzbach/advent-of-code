#include "ArgumentParser.hpp"
#include "Intcode.hpp"
#include "Text.hpp"
#include "Types.hpp"

#include "minbool.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

namespace AoC {
enum class DesiredOutput : U8 { False, True, Any };

// Bits: (MSB) I H G F E D C B A (LSB)
constexpr static U32 Bits = 9u;
constexpr static U32 States = 1u << Bits;

// TODO: try to use Debugging.h to do this.
#ifdef NDEBUG
constexpr static bool Debugging = false;
#else
constexpr static bool Debugging = true;
#endif

U32 toAhead(const std::string &string) {
  if (string.size() != Bits) {
    throw std::invalid_argument("String should have " + std::to_string(Bits) + " characters.");
  }
  U32 value = 0u;
  for (auto it = std::rbegin(string); it != std::rend(string); it++) {
    if (*it == '.') {
      value = (value << 1u) | 0u;
    } else if (*it == '#') {
      value = (value << 1u) | 1u;
    } else {
      throw std::invalid_argument("String should only have '.' and '#' characters.");
    }
  }
  return value;
}

constexpr U32 advance(const U32 ahead) {
  return (1u << (Bits - 1u)) | (ahead >> 1u);
}

class SafetyTables {
public:
  std::array<std::optional<bool>, States> safeIfJumps{};
  std::array<std::optional<bool>, States> safeIfDoesNotJump{};
};

bool isSafe(const SafetyTables &safetyTables, const U32 ahead);

bool isSafeIfJumps(const SafetyTables &safetyTables, const U32 ahead) {
  const auto &optionalCacheFromAhead = safetyTables.safeIfJumps[ahead];
  if (optionalCacheFromAhead) {
    return *optionalCacheFromAhead;
  }
  // Skips 3, lands in fourth. If there is a hole, then it is dead. Otherwise, recurse.
  if (((ahead >> 3u) & 1u) == 0u) {
    return false;
  }
  return isSafe(safetyTables, advance(advance(advance(advance(ahead)))));
}

bool isSafeIfDoesNotJump(const SafetyTables &safetyTables, const U32 ahead) {
  const auto &optionalCacheFromAhead = safetyTables.safeIfDoesNotJump[ahead];
  if (optionalCacheFromAhead) {
    return *optionalCacheFromAhead;
  }
  if ((ahead & 1u) == 0u) {
    return false;
  }
  return isSafe(safetyTables, advance(ahead));
}

bool isSafe(const SafetyTables &safetyTables, const U32 ahead) {
  return isSafeIfJumps(safetyTables, ahead) || isSafeIfDoesNotJump(safetyTables, ahead);
}

void main(ArgumentParser const &argumentParser) {
  auto originalIntcode = Intcode(readMemory(argumentParser.getPath()));
  const auto enterInstruction = [&originalIntcode](const std::string &string) {
    for (const auto c : string) {
      originalIntcode.addInput(c);
    }
    originalIntcode.addInput('\n');
  };
  if (argumentParser.getPart() == 1) {
    // D must be 1.
    enterInstruction("NOT A T");
    enterInstruction("OR T J");
    enterInstruction("NOT B T");
    enterInstruction("OR T J");
    enterInstruction("NOT C T");
    enterInstruction("OR T J");

    enterInstruction("AND D J");

    enterInstruction("WALK");
  } else {
    // There are 2^9 = 512 possible inputs.
    // We can simulate each one and find out the proper action.
    // Build what actions are safe.
    assert(toAhead("#########") == States - 1u);
    SafetyTables safetyTables;
    safetyTables.safeIfJumps[toAhead("#########")] = true;
    safetyTables.safeIfDoesNotJump[toAhead("#########")] = true;
    for (S32 i = States - 2u; i >= 0; i--) {
      safetyTables.safeIfJumps[i] = isSafeIfJumps(safetyTables, static_cast<U32>(i));
      safetyTables.safeIfDoesNotJump[i] = isSafeIfDoesNotJump(safetyTables, static_cast<U32>(i));
    }
    // Build the desired output array.
    std::array<std::optional<DesiredOutput>, States> desiredOutput{};
    desiredOutput[toAhead("#########")] = DesiredOutput::Any;
    for (U32 i = 0u; i < States; i++) {
      assert(safetyTables.safeIfJumps[i].has_value());
      const auto safeIfJumps = safetyTables.safeIfJumps[i].value();
      assert(safetyTables.safeIfDoesNotJump[i].has_value());
      const auto safeIfDoesNotJump = safetyTables.safeIfDoesNotJump[i].value();
      if (safeIfJumps && safeIfDoesNotJump) {
        desiredOutput[i] = DesiredOutput::Any;
      } else if (safeIfJumps) {
        desiredOutput[i] = DesiredOutput::True;
      } else if (safeIfDoesNotJump) {
        desiredOutput[i] = DesiredOutput::False;
      } else {
        // In this configuration, we die any way, so do not require any specific output.
        desiredOutput[i] = DesiredOutput::Any;
      }
    }
    assert(desiredOutput[toAhead("#########")] == DesiredOutput::Any);
    assert(desiredOutput[toAhead("####.#.#.")] == DesiredOutput::Any);
    for (U32 i = 0u; i < States; i++) {
      // If the next four tiles are ground, override "any" in favor of sticking to the ground.
      if (desiredOutput[i] == DesiredOutput::Any && (i & 0xFu) == 0xFu) {
        desiredOutput[i] = DesiredOutput::False;
      }
    }
    if (Debugging) {
      // Print the truth table.
      std::cout << "I H G F E D C B A | J !J | Y" << '\n';
      for (U32 i = 0u; i < States; i++) {
        for (S32 shiftAmount = Bits - 1u; shiftAmount >= 0; shiftAmount--) {
          if (static_cast<U32>(shiftAmount) < Bits - 1u) {
            std::cout << ' ';
          }
          std::cout << ((i >> static_cast<U32>(shiftAmount)) & 1u);
        }
        std::cout << " | ";
        std::cout << *safetyTables.safeIfJumps[i] << "  " << *safetyTables.safeIfDoesNotJump[i];
        std::cout << " | ";
        if (desiredOutput[i]) {
          if (*desiredOutput[i] == DesiredOutput::False) {
            std::cout << 'F';
          } else if (*desiredOutput[i] == DesiredOutput::True) {
            std::cout << 'T';
          } else {
            std::cout << '-';
          }
        }
        std::cout << '\n';
      }
    }
    // Assertions: we know what some of the values should be!
    assert(desiredOutput[toAhead("#########")] == DesiredOutput::False);
    assert(desiredOutput[toAhead(".########")] == DesiredOutput::True);
    assert(desiredOutput[toAhead("#.#######")] == DesiredOutput::Any);
    assert(desiredOutput[toAhead(".#.##.###")] == DesiredOutput::True);
    assert(desiredOutput[toAhead("#.#.##.##")] == DesiredOutput::False);
    assert(desiredOutput[toAhead("##.#.##.#")] == DesiredOutput::False);
    assert(desiredOutput[toAhead("####.#.#.")] == DesiredOutput::False);
    assert(desiredOutput[toAhead("###.##.##")] == DesiredOutput::False);
    assert(desiredOutput[toAhead("####.##.#")] == DesiredOutput::False);
    // Minimize the boolean function we have.
    std::vector<U16> on;
    std::vector<U16> dontCare;
    for (U32 i = 0u; i < States; i++) {
      if (desiredOutput[i].value() == DesiredOutput::True) {
        on.push_back(i);
      } else if (desiredOutput[i].value() == DesiredOutput::Any) {
        dontCare.push_back(i);
      }
    }
    const auto solution = minbool::minimize_boolean<Bits>(on, dontCare);
    if (Debugging) {
      std::cout << '\n';
      std::cout << "I H G F E D C B A" << '\n';
      for (const auto term : solution) {
        for (U32 i = 0; i < Bits; i++) {
          if (i > 0) {
            std::cout << ' ';
          }
          if (term[i] == 2u) {
            std::cout << '-';
          } else {
            std::cout << term[i];
          }
        }
        std::cout << '\n';
      }
      std::cout << '\n';
    }
    // Enter the program we found from the boolean minimization.
    for (const auto term : solution) {
      std::vector<U32> zeros;
      std::vector<U32> ones;
      for (U32 i = 0; i < Bits; i++) {
        if (term[i] == 0u) {
          zeros.push_back(i);
        } else if (term[i] == 1u) {
          ones.push_back(i);
        }
      }
      if (zeros.size() != 1) {
        throw std::runtime_error("Expected exactly one zero.");
      }
      enterInstruction("NOT " + std::string(1, static_cast<char>('A' + zeros[0])) + " T");
      for (const auto oneIndex : ones) {
        enterInstruction("AND " + std::string(1, static_cast<char>('A' + oneIndex)) + " T");
      }
      enterInstruction("OR T J");
    }
    enterInstruction("RUN");
  }
  const auto finalState = originalIntcode.run();
  assert(finalState == IntcodeState::Halted);
  while (originalIntcode.getOutputBufferLength() > 1) {
    std::cout << static_cast<char>(originalIntcode.getOutput());
  }
  std::cout << originalIntcode.getOutput() << '\n';
}
} // namespace AoC

#include "Main.inl"
