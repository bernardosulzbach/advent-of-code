#include <boost/process.hpp>

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include "Filesystem.hpp"
#include "Text.hpp"

namespace AoC {
struct [[nodiscard]] RunStatistics {
  U32 tested = 0;
  U32 passed = 0;

  [[nodiscard]] std::string generateSummary() const noexcept {
    std::stringstream stream;
    stream << "Passed " << passed << " out of " << tested << " tests.";
    return stream.str();
  }
};

class [[nodiscard]] RunSpecification {
public:
  RunSpecification(U32 runDay, bool runIsSample, U32 runPart, U32 runNumber) noexcept
      : day(runDay), isSample(runIsSample), part(runPart), number(runNumber) {
  }

  void run(RunStatistics &runStatistics) const {
    const auto command = "./" + toZeroPaddedString(day, 2) + " " + getInputPath().native() + " " + std::to_string(part);
    const auto outputPath = getOutputPath();
    std::cout << "Testing (" << command << ") against (" << outputPath.native() << ")." << '\n';
    const auto actualOutput = getOutput(command);
    const auto expectedOutput = readFile(outputPath);
    if (actualOutput == expectedOutput) {
      runStatistics.passed++;
    } else {
      std::cout << "Output is wrong: " << actualOutput << " != " << expectedOutput << '\n';
    }
    runStatistics.tested++;
  }

  [[nodiscard]] constexpr auto operator<=>(RunSpecification const &rhs) const noexcept = default;

private:
  [[nodiscard]] std::filesystem::path getInputPath() const noexcept {
    auto filename = toZeroPaddedString(day, 2);
    if (isSample) {
      filename += "-" + std::to_string(part) + "-" + std::to_string(number);
    }
    filename += ".txt";
    const auto subdirectory = isSample ? "samples" : "puzzles";
    const auto inputPath = std::filesystem::path("..") / "input" / subdirectory / filename;
    return inputPath;
  }

  [[nodiscard]] std::filesystem::path getOutputPath() const noexcept {
    auto filename = toZeroPaddedString(day, 2) + "-" + std::to_string(part);
    if (isSample) {
      filename += "-" + std::to_string(number);
    }
    filename += ".txt";
    const auto subdirectory = isSample ? "samples" : "puzzles";
    const auto outputPath = std::filesystem::path("..") / "output" / subdirectory / filename;
    return outputPath;
  }

  [[nodiscard]] static std::string getOutput(const std::string &command) {
    boost::process::ipstream pipeStream;
    boost::process::child child(command, boost::process::std_out > pipeStream);
    std::string result;
    std::string line;
    while (pipeStream && std::getline(pipeStream, line) && !line.empty()) {
      result += line + '\n';
    }
    child.wait();
    boost::trim(result);
    return result;
  }

  U32 day;
  bool isSample;
  U32 part;
  U32 number;
};

[[nodiscard]] std::set<RunSpecification> generateRunSpecifications() noexcept;
} // namespace AoC
