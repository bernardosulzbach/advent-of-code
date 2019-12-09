#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

struct TestsStatistics {
  int tested = 0;
  int passed = 0;

  [[nodiscard]] std::string generateSummary() const {
    std::stringstream stream;
    stream << "Passed " << passed << " out of " << tested << " tests.";
    return stream.str();
  }
};

std::string getOutput(const std::string &command) {
  boost::process::ipstream pipe_stream;
  boost::process::child child(command, boost::process::std_out > pipe_stream);
  std::string result;
  while (pipe_stream) {
    std::string reading;
    pipe_stream >> reading;
    result += reading;
  }
  child.wait();
  boost::trim(result);
  return result;
}

std::string readFile(const std::filesystem::path &path) {
  std::ostringstream buffer;
  std::ifstream input(path.native());
  buffer << input.rdbuf();
  auto asString = buffer.str();
  boost::trim(asString);
  return asString;
}
std::string toPaddedDayString(int day) {
  std::stringstream stream;
  stream << std::setfill('0') << std::setw(2) << day;
  return stream.str();
}

std::filesystem::path getInputPath(int day, bool isSample, int part, int number) {
  auto filename = toPaddedDayString(day);
  if (isSample) {
    filename += "-" + std::to_string(part) + "-" + std::to_string(number);
  }
  filename += ".txt";
  const auto subdirectory = isSample ? "samples" : "puzzles";
  const auto inputPath = std::filesystem::path("..") / "input" / subdirectory / filename;
  return inputPath;
}

std::filesystem::path getOutputPath(int day, bool isSample, int part, int number) {
  auto filename = toPaddedDayString(day) + "-" + std::to_string(part);
  if (isSample) {
    filename += "-" + std::to_string(number);
  }
  filename += ".txt";
  const auto subdirectory = isSample ? "samples" : "puzzles";
  const auto outputPath = std::filesystem::path("..") / "output" / subdirectory / filename;
  return outputPath;
}

void test(int day, bool isSample, int part, int number, TestsStatistics &testsStatistics) {
  const auto inputPath = getInputPath(day, isSample, part, number);
  const auto outputPath = getOutputPath(day, isSample, part, number);
  const auto command = "./" + toPaddedDayString(day) + " " + inputPath.native() + " " + std::to_string(part);
  std::cout << "Testing (" << command << ") against (" << outputPath.native() << ")." << '\n';
  const auto actualOutput = getOutput(command);
  const auto expectedOutput = readFile(outputPath);
  if (actualOutput == expectedOutput) {
    testsStatistics.passed++;
  } else {
    std::cout << "Output is wrong: " << actualOutput << " != " << expectedOutput << '\n';
  }
  testsStatistics.tested++;
}

int main() {
  TestsStatistics testsStatistics;

  for (int part = 1; part <= 2; part++) {
    for (int number = 1; number <= 3; number++) {
      test(3, true, part, number, testsStatistics);
    }
  }

  test(6, true, 1, 1, testsStatistics);
  test(6, true, 2, 1, testsStatistics);

  test(7, true, 1, 1, testsStatistics);
  test(7, true, 1, 2, testsStatistics);
  test(7, true, 1, 3, testsStatistics);
  test(7, true, 2, 1, testsStatistics);
  test(7, true, 2, 2, testsStatistics);

  for (int day = 1; day <= 7; day++) {
    test(day, false, 1, 1, testsStatistics);
    test(day, false, 2, 1, testsStatistics);
  }

  std::cout << testsStatistics.generateSummary() << '\n';
}
