#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

#include "../src/Direction.hpp"
#include "../src/IO.hpp"
#include "../src/Math.hpp"
#include "../src/Point.hpp"
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

BOOST_AUTO_TEST_CASE(directionConversionTest) {
  for (auto const character : {'N', 'E', 'S', 'W'}) {
    BOOST_CHECK(character == directionToCharacter(characterToDirection(character)));
  }
}

BOOST_AUTO_TEST_CASE(rotateAroundTest) {
  {
    auto const point = Point<S32>{2, 0};
    auto const pivot = Point<S32>{1, 0};
    BOOST_CHECK(Point<S32>(1, 1) == point.rotateAround(pivot, 90));
  }
  {
    auto const point = Point<S32>{3, 3};
    auto const pivot = Point<S32>{2, 1};
    BOOST_CHECK(Point<S32>(4, 0) == point.rotateAround(pivot, -90));
  }
}

BOOST_AUTO_TEST_CASE(euclideanModuloTest) {
  auto const lowest = std::numeric_limits<S64>::lowest();
  BOOST_CHECK(AoC::euclideanModulo(static_cast<S64>(-1), lowest) == -(lowest + 1));
  BOOST_CHECK(AoC::euclideanModulo(static_cast<S64>(0), lowest) == 0);
  BOOST_CHECK(AoC::euclideanModulo(static_cast<S64>(1), lowest) == 1);
  auto const max = std::numeric_limits<S64>::max();
  BOOST_CHECK(AoC::euclideanModulo(static_cast<S64>(-1), max) == max - 1);
  BOOST_CHECK(AoC::euclideanModulo(static_cast<S64>(0), max) == 0);
  BOOST_CHECK(AoC::euclideanModulo(static_cast<S64>(1), max) == 1);
}
