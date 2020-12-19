#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../src/IO.hpp"
#include "../src/Ranges.hpp"
#include "../src/Text.hpp"

BOOST_AUTO_TEST_CASE(splitAtTest) {
  std::vector<std::string> expectedResult{"a", "b", "c"};
  BOOST_CHECK(splitAt("a, b, c", ", ") == expectedResult);
}

BOOST_AUTO_TEST_CASE(allCombinationsOfLengthOneTest) {
  std::vector<int> range{1, 2, 3};
  constexpr std::size_t combinationSize = 1;
  // TODO: consider a push_back, back_inserter, or something else to remove these loops.
  std::vector<std::array<int, combinationSize>> combinations;
  for (auto const combination : AoC::allCombinations<combinationSize>(range)) {
    combinations.push_back(combination);
  }
  std::vector<std::array<int, combinationSize>> expectedCombinations{{1}, {2}, {3}};
  BOOST_CHECK(expectedCombinations == combinations);
}

BOOST_AUTO_TEST_CASE(allCombinationsOfLengthInBetweenSizeTest) {
  std::vector<int> range{1, 2, 3};
  constexpr std::size_t combinationSize = 2;
  std::vector<std::array<int, combinationSize>> combinations;
  for (auto const combination : AoC::allCombinations<combinationSize>(range)) {
    combinations.push_back(combination);
  }
  std::vector<std::array<int, combinationSize>> expectedCombinations{{1, 2}, {1, 3}, {2, 3}};
  BOOST_CHECK(expectedCombinations == combinations);
}

BOOST_AUTO_TEST_CASE(allCombinationsOfTotalLengthTest) {
  std::array<int, 3> const range{1, 2, 3};
  constexpr std::size_t combinationSize = 3;
  std::vector<std::array<int, combinationSize>> combinations;
  for (auto const combination : AoC::allCombinations<combinationSize>(range)) {
    combinations.push_back(combination);
  }
  std::vector<std::array<int, combinationSize>> expectedCombinations{{1, 2, 3}};
  BOOST_CHECK(expectedCombinations == combinations);
}

BOOST_AUTO_TEST_CASE(printTest) {
  {
    std::stringstream stream;
    std::vector<bool> vector;
    AoC::print(stream, vector, "");
    BOOST_CHECK(stream.str() == "\n");
  }
  {
    std::stringstream stream;
    std::vector<bool> vector{false};
    AoC::print(stream, vector, "");
    BOOST_CHECK(stream.str() == "0\n");
  }
  {
    std::stringstream stream;
    std::vector<bool> vector{false, true};
    AoC::print(stream, vector, "");
    BOOST_CHECK(stream.str() == "01\n");
  }
  {
    std::stringstream stream;
    std::vector<bool> vector{false, true};
    AoC::print(stream, vector, ", ");
    BOOST_CHECK(stream.str() == "0, 1\n");
  }
}
