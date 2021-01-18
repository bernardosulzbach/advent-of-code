#include "Runner.hpp"

namespace AoC {
[[nodiscard]] std::set<RunSpecification> generateRunSpecifications() noexcept {
  std::set<RunSpecification> runSpecifications;
  for (int part = 1; part <= 2; part++) {
    for (int number = 1; number <= 3; number++) {
      runSpecifications.emplace(3, true, part, number);
    }
  }

  runSpecifications.emplace(6, true, 1, 1);
  runSpecifications.emplace(6, true, 2, 1);

  runSpecifications.emplace(7, true, 1, 1);
  runSpecifications.emplace(7, true, 1, 2);
  runSpecifications.emplace(7, true, 1, 3);
  runSpecifications.emplace(7, true, 2, 1);
  runSpecifications.emplace(7, true, 2, 2);

  runSpecifications.emplace(10, true, 1, 1);
  runSpecifications.emplace(10, true, 2, 1);

  runSpecifications.emplace(12, true, 2, 1);
  runSpecifications.emplace(12, true, 2, 2);

  for (int part = 1; part <= 2; part++) {
    for (int number = 1; number <= 4; number++) {
      runSpecifications.emplace(14, true, part, number);
    }
  }

  for (int number = 1; number <= 4; number++) {
    runSpecifications.emplace(16, true, 1, number);
  }

  for (int day = 1; day <= 17; day++) {
    runSpecifications.emplace(day, false, 1, 1);
    // TODO: remove this once day 13 is solved.
    if (day == 13 || day == 16) {
      continue;
    }
    runSpecifications.emplace(day, false, 2, 1);
  }
  return runSpecifications;
}
} // namespace AoC

#include "Runner.inl"