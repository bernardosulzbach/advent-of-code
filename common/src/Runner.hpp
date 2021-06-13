#include <filesystem>
#include <set>
#include <string>

#include "Time.hpp"

namespace AoC {
class [[nodiscard]] RunStatistics {
public:
  void update(bool const passedTests, Duration const duration) noexcept;

  friend std::ostream &operator<<(std::ostream &stream, RunStatistics const &runStatistics) noexcept;

private:
  U32 tested = 0;
  U32 passed = 0;
  Duration totalDuration;
};

class [[nodiscard]] RunSpecification {
public:
  RunSpecification(U32 runDay, bool runIsSample, U32 runPart, U32 runNumber) noexcept;

  void run(RunStatistics &runStatistics) const;

  [[nodiscard]] constexpr auto operator<=>(RunSpecification const &rhs) const noexcept = default;

private:
  [[nodiscard]] std::filesystem::path getInputPath() const noexcept;

  [[nodiscard]] std::filesystem::path getOutputPath() const noexcept;

  [[nodiscard]] static std::string getOutput(const std::string &command);

  U32 day;
  bool isSample;
  U32 part;
  U32 number;
};

[[nodiscard]] std::set<RunSpecification> generateRunSpecifications() noexcept;
} // namespace AoC
