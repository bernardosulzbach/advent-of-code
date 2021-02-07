#include "ArgumentParser.hpp"
#include "Casts.hpp"
#include "Containers.hpp"
#include "ContextFreeGrammar.hpp"
#include "IO.hpp"
#include "Logic.hpp"
#include "Types.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stack>
#include <vector>

namespace AoC {
void parseProductionLine(std::vector<Production> &productions, std::string const &line) {
  char character;
  std::stringstream stream(line);
  std::string name;
  while (stream >> character && character != ':') {
    name += character;
  }
  std::vector<std::string> products;
  std::string currentToken;
  auto const flush = [&productions, &name, &products]() {
    assert(!products.empty());
    productions.emplace_back(name, products);
    products.clear();
  };
  while (stream >> currentToken) {
    assert(!currentToken.empty());
    if (currentToken == "|") {
      flush();
    } else {
      if (currentToken.starts_with('"')) {
        assert(currentToken.size() >= 3);
        currentToken = currentToken.substr(1, currentToken.size() - 2);
      }
      products.push_back(currentToken);
    }
  }
  flush();
}

void main(ArgumentParser const &argumentParser) {
  auto stream = std::ifstream(argumentParser.getPath());
  std::vector<AoC::Production> productions;
  auto const lines = AoC::readLines(stream);
  auto lineIterator = std::begin(lines);
  for (; lineIterator != std::end(lines) && !lineIterator->empty(); ++lineIterator) {
    AoC::parseProductionLine(productions, *lineIterator);
  }
  ++lineIterator; // Skip the blank line.
  auto const isZeroProduction = [](auto const &production) { return production.getName() == "0"; };
  assert(std::ranges::count_if(productions, isZeroProduction) == 1);
  if (argumentParser.getPart() == 2) {
    std::erase_if(productions, [](auto const &production) { return AoC::isAnyOf(production.getName(), "8", "11"); });
    productions.emplace_back("8", std::vector<std::string>{"42"});
    productions.emplace_back("8", std::vector<std::string>{"42", "8"});
    productions.emplace_back("11", std::vector<std::string>{"42", "31"});
    productions.emplace_back("11", std::vector<std::string>{"42", "11", "31"});
  }
  AoC::ContextFreeGrammar contextFreeGrammar(productions);
  auto const zeroProductionIterator = std::ranges::find_if(productions, isZeroProduction);
  auto const zeroProductionIndex = std::ranges::distance(std::ranges::begin(productions), zeroProductionIterator);
  AoC::ContextFreeGrammarRecognizer recognizer(contextFreeGrammar, unsignedCast<Unchecked>(zeroProductionIndex));
  std::cout << std::count_if(lineIterator, std::end(lines), [&recognizer](auto const &it) {
    std::vector<std::string> words;
    for (auto const character : it) {
      words.emplace_back(1, character);
    }
    return recognizer.isRecognized(words);
  }) << '\n';
}
} // namespace AoC

#include "Main.inl"
