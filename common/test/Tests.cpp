#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <iostream>

#include "../src/ContextFreeGrammar.hpp"
#include "../src/Direction.hpp"
#include "../src/IO.hpp"
#include "../src/Logic.hpp"
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

BOOST_AUTO_TEST_CASE(vectorNormsTest) {
  BOOST_CHECK((Vector<S32, 2>{0, 0}.getL1Norm() == 0));
  BOOST_CHECK((Vector<S32, 2>{0, 1}.getL1Norm() == 1));
  BOOST_CHECK((Vector<S32, 2>{1, 0}.getL1Norm() == 1));
  BOOST_CHECK((Vector<S32, 2>{1, 1}.getL1Norm() == 2));
  BOOST_CHECK((Vector<S32, 2>{1, 2}.getL1Norm() == 3));

  BOOST_CHECK((Vector<S32, 2>{0, 0}.getL2NormSquare() == 0));
  BOOST_CHECK((Vector<S32, 2>{0, 1}.getL2NormSquare() == 1));
  BOOST_CHECK((Vector<S32, 2>{1, 0}.getL2NormSquare() == 1));
  BOOST_CHECK((Vector<S32, 2>{1, 1}.getL2NormSquare() == 2));
  BOOST_CHECK((Vector<S32, 2>{1, 2}.getL2NormSquare() == 5));

  BOOST_CHECK((Vector<S32, 2>{0, 0}.getL2Norm() == 0));
  BOOST_CHECK((Vector<S32, 2>{0, 1}.getL2Norm() == 1));
  BOOST_CHECK((Vector<S32, 2>{1, 0}.getL2Norm() == 1));
  BOOST_CHECK((Vector<S32, 2>{1, 1}.getL2Norm() == std::sqrt(2)));
  BOOST_CHECK((Vector<S32, 2>{1, 2}.getL2Norm() == std::sqrt(5)));
}

BOOST_AUTO_TEST_CASE(pointDirectNeighorsTest) {
  auto const checkDirectNeighborsInvariants = [](auto const point) {
    const auto directNeighbors = point.getDirectNeighbors();
    std::ranges::is_sorted(directNeighbors);
    for (auto const neighbor : directNeighbors) {
      BOOST_CHECK_EQUAL(point.to(neighbor).getL1Norm(), 1);
    }
  };
  checkDirectNeighborsInvariants(Point<S32, 2>{});
  checkDirectNeighborsInvariants(Point<S32, 3>{});
  checkDirectNeighborsInvariants(Point<S32, 4>{});
}

BOOST_AUTO_TEST_CASE(pointNeighorsTest) {
  auto const checkNeighborsInvariants = [](auto const point) {
    const auto neighbors = point.getNeighbors();
    std::ranges::is_sorted(neighbors);
  };
  checkNeighborsInvariants(Point<S32, 2>{});
  checkNeighborsInvariants(Point<S32, 3>{});
  checkNeighborsInvariants(Point<S32, 4>{});
}

BOOST_AUTO_TEST_CASE(directionConversionTest) {
  for (auto const character : {'N', 'E', 'S', 'W'}) {
    BOOST_CHECK(character == directionToCharacter(characterToDirection(character)));
  }
}

BOOST_AUTO_TEST_CASE(rotateAroundTest) {
  {
    auto const point = Point<S32, 2>{2, 0};
    auto const pivot = Point<S32, 2>{1, 0};
    BOOST_CHECK((Point<S32, 2>(1, 1) == point.rotateAround(pivot, 90)));
  }
  {
    auto const point = Point<S32, 2>{3, 3};
    auto const pivot = Point<S32, 2>{2, 1};
    BOOST_CHECK((Point<S32, 2>(4, 0) == point.rotateAround(pivot, -90)));
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

BOOST_AUTO_TEST_CASE(isAnyOfTest) {
  BOOST_CHECK_EQUAL(AoC::isAnyOf(0, 0), true);
  BOOST_CHECK_EQUAL(AoC::isAnyOf(0, 1), false);
  BOOST_CHECK_EQUAL(AoC::isAnyOf(0, 0, 0), true);
  BOOST_CHECK_EQUAL(AoC::isAnyOf(0, 0, 1), true);
  BOOST_CHECK_EQUAL(AoC::isAnyOf(0, 1, 0), true);
  BOOST_CHECK_EQUAL(AoC::isAnyOf(0, 1, 1), false);
  BOOST_CHECK_EQUAL(AoC::isAnyOf(0, 0, 0, 0), true);
}

BOOST_AUTO_TEST_CASE(contextFreeGrammarRecognizerTest) {
  std::vector<AoC::Production> productions;
  productions.emplace_back("P", std::vector<std::string>{"S"});
  productions.emplace_back("S", std::vector<std::string>{"S", "+", "M"});
  productions.emplace_back("S", std::vector<std::string>{"M"});
  productions.emplace_back("M", std::vector<std::string>{"M", "*", "T"});
  productions.emplace_back("M", std::vector<std::string>{"T"});
  productions.emplace_back("T", std::vector<std::string>{"x"});
  productions.emplace_back("T", std::vector<std::string>{"y"});
  productions.emplace_back("T", std::vector<std::string>{"z"});
  AoC::ContextFreeGrammar contextFreeGrammar(productions);
  AoC::ContextFreeGrammarRecognizer recognizer(contextFreeGrammar, 0);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"x"}), true);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"w"}), false);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"x", "y"}), false);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"x", "+"}), false);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"x", "+", "y"}), true);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"x", "+", "*"}), false);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"x", "+", "y", "*"}), false);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"x", "+", "y", "*", "z"}), true);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"x", "+", "y", "*", "z", "*"}), false);
  BOOST_CHECK_EQUAL(recognizer.isRecognized(std::vector<std::string>{"x", "+", "y", "*", "z", "*", "x"}), true);
}
