#pragma once

#include "Casts.hpp"
#include "Point.hpp"
#include "Types.hpp"

#include <stdexcept>
#include <string>
#include <vector>

class Screen {
  std::vector<std::string> pixels;
  Size width = 0;
  Size height = 0;
  S64 score = 0;

  static constexpr char DefaultCharacter = ' ';

  void resize(Size const x, Size const y) noexcept {
    width = std::max(width, x);
    height = std::max(height, y);
    pixels.resize(height + 1);
    for (auto &line : pixels) {
      line.resize(width + 1, DefaultCharacter);
    }
  }

public:
  void setScore(S64 const newScore) noexcept {
    score = newScore;
  }

  [[nodiscard]] S64 getScore() const noexcept {
    return score;
  }

  [[nodiscard]] Point<Size, 2> getPosition(char symbol) const {
    for (Size i = 0; i < height; i++) {
      for (Size j = 0; j < width; j++) {
        if (pixels[i][j] == symbol) {
          return {j, i};
        }
      }
    }
    throw std::runtime_error("Symbol '" + std::string(1, symbol) + "' not found.");
  }

  void paint(Size const x, Size const y, char const c) {
    resize(x, y);
    pixels[y][x] = c;
  }

  [[nodiscard]] std::string toString() const noexcept {
    std::string result;
    result += "Score: " + std::to_string(score);
    for (const auto &line : pixels) {
      result += '\n';
      result += line;
    }
    return result;
  }
};
