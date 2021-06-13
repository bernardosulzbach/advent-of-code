#include "Runner.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <boost/process/pipe.hpp>

#include "Filesystem.hpp"
#include "Text.hpp"

namespace AoC {
void RunStatistics::update(bool const passedTests, Duration const duration) noexcept {
  tested++;
  if (passedTests) {
    passed++;
  }
  totalDuration += duration;
}

std::ostream &operator<<(std::ostream &stream, RunStatistics const &runStatistics) noexcept {
  return (stream << "Passed " << runStatistics.passed << " out of " << runStatistics.tested << " tests in "
                 << runStatistics.totalDuration << ".");
}

RunSpecification::RunSpecification(U32 runDay, bool runIsSample, U32 runPart, U32 runNumber) noexcept
    : day(runDay), isSample(runIsSample), part(runPart), number(runNumber) {
}

void RunSpecification::run(RunStatistics &runStatistics) const {
  const auto command = "./" + toZeroPaddedString(day, 2) + " " + getInputPath().native() + " " + std::to_string(part);
  const auto outputPath = getOutputPath();
  std::cout << "Testing (" << command << ") against (" << outputPath.native() << "). ";
  Timer timer;
  const auto actualOutput = getOutput(command);
  const auto duration = timer.getElapsed();
  std::cout << "Done after " << duration << ".\n";
  std::ifstream outputStream(outputPath);
  const auto expectedOutput = readFile(outputStream);
  const auto passedTests = actualOutput == expectedOutput;
  if (!passedTests) {
    std::cout << "Output is wrong: " << actualOutput << " != " << expectedOutput << '\n';
  }
  runStatistics.update(passedTests, duration);
}

[[nodiscard]] std::filesystem::path RunSpecification::getInputPath() const noexcept {
  auto filename = toZeroPaddedString(day, 2);
  if (isSample) {
    filename += "-" + std::to_string(part) + "-" + std::to_string(number);
  }
  filename += ".txt";
  const auto subdirectory = isSample ? "samples" : "puzzles";
  const auto inputPath = std::filesystem::path("..") / "input" / subdirectory / filename;
  return inputPath;
}

[[nodiscard]] std::filesystem::path RunSpecification::getOutputPath() const noexcept {
  auto filename = toZeroPaddedString(day, 2) + "-" + std::to_string(part);
  if (isSample) {
    filename += "-" + std::to_string(number);
  }
  filename += ".txt";
  const auto subdirectory = isSample ? "samples" : "puzzles";
  return std::filesystem::path("..") / "output" / subdirectory / filename;
}

[[nodiscard]] std::string RunSpecification::getOutput(const std::string &command) {
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
} // namespace AoC
