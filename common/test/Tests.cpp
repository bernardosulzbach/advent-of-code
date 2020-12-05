#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../src/Text.hpp"

BOOST_AUTO_TEST_CASE(splitAtTest) {
  std::vector<std::string> expectedResult{"a", "b", "c"};
  BOOST_CHECK(splitAt("a, b, c", ", ") == expectedResult);
}
