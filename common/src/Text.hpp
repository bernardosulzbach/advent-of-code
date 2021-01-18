#include "Types.hpp"

#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> splitAt(std::string_view const string, std::string_view const separator);

namespace AoC {
std::string toZeroPaddedString(U64 day, U64 length);
}