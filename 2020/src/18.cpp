#include "ArgumentParser.hpp"
#include "Containers.hpp"
#include "IO.hpp"
#include "Logic.hpp"
#include "Types.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stack>

enum class TokenType : U8 { None, OpenParenthesis, CloseParenthesis, Plus, Times, Number };

using IntegerType = S64;

struct Token {
  TokenType type = TokenType::None;
  IntegerType value = -1;

  [[nodiscard]] constexpr bool isOperator() const noexcept {
    return AoC::isAnyOf(type, TokenType::Plus, TokenType::Times);
  }

  [[nodiscard]] constexpr Token evaluate(Token const rhs, Token const lhs) const noexcept {
    assert(isOperator());
    if (type == TokenType::Plus) {
      return Token{TokenType::Number, lhs.value + rhs.value};
    }
    assert(type == TokenType::Times);
    return Token{TokenType::Number, lhs.value * rhs.value};
  }

  [[nodiscard]] constexpr explicit operator bool() const noexcept {
    return type != TokenType::None;
  }

  friend std::ostream &operator<<(std::ostream &stream, Token const token) noexcept {
    assert(token);
    if (token.type == TokenType::OpenParenthesis) {
      stream << '(';
    } else if (token.type == TokenType::CloseParenthesis) {
      stream << ')';
    } else if (token.type == TokenType::Plus) {
      stream << '+';
    } else if (token.type == TokenType::Times) {
      stream << '*';
    } else if (token.type == TokenType::Number) {
      stream << token.value;
    }
    return stream;
  }
};

[[nodiscard]] Token tokenFromSymbol(char symbol) {
  if (symbol == '(') {
    return Token{TokenType::OpenParenthesis};
  }
  if (symbol == ')') {
    return Token{TokenType::CloseParenthesis};
  }
  if (symbol == '+') {
    return Token{TokenType::Plus};
  }
  if (symbol == '*') {
    return Token{TokenType::Times};
  }
  throw std::runtime_error("Cannot construct token from symbol: " + std::string(1, symbol) + ".");
}

[[nodiscard]] Token readToken(std::stringstream &stream) noexcept {
  char character;
  if (!(stream >> character)) {
    return {};
  }
  if (!std::isdigit(character)) {
    return tokenFromSymbol(character);
  }
  IntegerType value = character - '0';
  while (stream >> character) {
    if (std::isdigit(character)) {
      value = 10 * value + (character - '0');
    } else {
      assert(stream.unget());
      break;
    }
  }
  return Token{TokenType::Number, value};
}

// If additionsFirst, performs an addition only pass first.
void reduce(std::vector<Token> &tokens, Size const start, bool const additionsFirst) noexcept {
  std::vector<Token> reduction;
  for (auto read = tokens.begin() + start; read != tokens.end(); read++) {
    if (!reduction.empty() && read->type == TokenType::Number) {
      if (!additionsFirst || reduction.back().type == TokenType::Plus) {
        auto const operatorToken = AoC::extractBack(reduction);
        auto const lhs = AoC::extractBack(reduction);
        reduction.push_back(operatorToken.evaluate(lhs, *read));
        continue;
      }
    }
    reduction.push_back(*read);
  }
  if (additionsFirst) {
    while (reduction.size() >= 3) {
      auto const rhs = AoC::extractBack(reduction);
      auto const operatorToken = AoC::extractBack(reduction);
      assert(operatorToken.type == TokenType::Times);
      reduction.back() = operatorToken.evaluate(reduction.back(), rhs);
    }
  }
  assert(reduction.size() == 1);
  tokens[start] = reduction.front();
  tokens.erase(tokens.begin() + start + 1, tokens.end());
}

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
    IntegerType total = 0;
    for (auto const &line : AoC::readLines(stream)) {
      Token token;
      std::stringstream lineStream(line);
      std::vector<Token> tokens;
      std::stack<Size> openingBrackets;
      while ((token = readToken(lineStream))) {
        if (token.type == TokenType::CloseParenthesis) {
          assert(!openingBrackets.empty());
          reduce(tokens, AoC::extractTop(openingBrackets) + 1, part == 2);
          tokens.back() = AoC::extractBack(tokens);
        } else if (AoC::isAnyOf(token.type, TokenType::OpenParenthesis, TokenType::Number) || token.isOperator()) {
          if (token.type == TokenType::OpenParenthesis) {
            openingBrackets.push(tokens.size());
          }
          tokens.push_back(token);
        }
      }
      assert(openingBrackets.empty());
      reduce(tokens, 0, part == 2);
      assert(tokens.size() == 1);
      total += tokens.back().value;
    }
    std::cout << total << '\n';
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}