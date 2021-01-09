#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "Point.hpp"
#include "Types.hpp"

class Screen {
  std::vector<std::string> pixels;
  std::size_t width = 0;
  std::size_t height = 0;
  S64 score = 0;

  constexpr static char DefaultCharacter = ' ';

  void resize(std::size_t x, std::size_t y) {
    width = std::max(width, x);
    height = std::max(height, y);
    pixels.resize(height + 1);
    for (auto &line : pixels) {
      line.resize(width + 1, DefaultCharacter);
    }
  }

public:
  void setScore(S64 newScore) {
    score = newScore;
  }

  [[nodiscard]] S64 getScore() const {
    return score;
  }

  [[nodiscard]] Point<S64, 2> getPosition(char symbol) const {
    for (std::size_t i = 0; i < height; i++) {
      for (std::size_t j = 0; j < width; j++) {
        if (pixels[i][j] == symbol) {
          return Point<S64, 2>(j, i);
        }
      }
    }
    throw std::runtime_error("Symbol '" + std::string(1, symbol) + "' not found.");
  }

  void paint(S64 x, S64 y, char c) {
    resize(x, y);
    pixels[y][x] = c;
  }

  [[nodiscard]] std::string toString() const {
    std::string result;
    result += "Score: " + std::to_string(score);
    for (const auto &line : pixels) {
      result += '\n';
      result += line;
    }
    return result;
  }
};
