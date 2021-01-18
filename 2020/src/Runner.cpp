#include "Runner.hpp"

namespace AoC {
[[nodiscard]] std::set<RunSpecification> generateRunSpecifications() noexcept {
  std::set<RunSpecification> runSpecifications;
  for (U32 day = 1; day <= 19; day++) {
    for (U32 part = 1; part <= 2; part++) {
      runSpecifications.emplace(day, false, part, 1);
    }
  }
  return runSpecifications;
}
} // namespace AoC

#include "Runner.inl"