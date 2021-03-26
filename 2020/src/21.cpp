#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Text.hpp"
#include "Types.hpp"

#include <algorithm>

#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace AoC {
void main(std::istream &stream, U32 const part) {
  std::unordered_map<std::string, std::unordered_set<std::string>> mayBeIn;
  auto const parseLine = [](auto const &line) noexcept {
    auto const openingParenthesisIndex = line.find('(');
    auto const ingredientsSubstring = std::string_view(line.data(), openingParenthesisIndex - 1);
    auto const ingredientsVector = splitAt(ingredientsSubstring, " ");
    auto const ingredientsSet = std::unordered_set(std::begin(ingredientsVector), std::end(ingredientsVector));
    auto const allergensIndex = openingParenthesisIndex + 10;
    auto const allergensSubstring = std::string_view(line.data() + allergensIndex, line.size() - 1 - allergensIndex);
    auto const allergensVector = splitAt(allergensSubstring, ", ");
    return std::make_pair(ingredientsSet, allergensVector);
  };
  std::unordered_map<std::string, bool> mayContainAnAllergen;
  std::unordered_map<std::string, U32> ingredientAppearances;
  for (auto const &line : readLines(stream)) {
    auto const [ingredients, allergens] = parseLine(line);
    for (auto const &ingredient : ingredients) {
      ++ingredientAppearances[ingredient];
      mayContainAnAllergen[ingredient] = false;
    }
    for (auto const &allergen : allergens) {
      if (!mayBeIn.contains(allergen)) {
        mayBeIn.emplace(allergen, ingredients);
      } else {
        std::unordered_set<std::string> remainingSuspects;
        auto &suspects = mayBeIn[allergen];
        for (auto const &suspect : suspects) {
          if (ingredients.contains(suspect)) {
            remainingSuspects.insert(suspect);
          }
        }
        suspects = remainingSuspects;
      }
    }
  }
  for (auto const &[allergen, suspects] : mayBeIn) {
    for (auto const &suspect : suspects) {
      mayContainAnAllergen[suspect] = true;
    }
  }
  if (part == 1) {
    U32 count = 0;
    for (auto const &[ingredient, mayContain] : mayContainAnAllergen) {
      if (!mayContain) {
        count += ingredientAppearances[ingredient];
      }
    }
    std::cout << count << "\n";
  } else {
    std::vector<std::pair<std::string, std::string>> allergenAndIngredient;
    while (allergenAndIngredient.size() != mayBeIn.size()) {
      auto const removeIngredient = [&mayBeIn](auto const ingredient) noexcept {
        for (auto &[_, suspects] : mayBeIn) {
          suspects.erase(ingredient);
        }
      };
      for (auto const &[allergen, suspects] : mayBeIn) {
        if (suspects.size() == 1) {
          auto const ingredient = *std::begin(suspects);
          allergenAndIngredient.emplace_back(allergen, ingredient);
          removeIngredient(ingredient);
          break;
        }
      }
    }
    std::ranges::sort(allergenAndIngredient);
    auto first = true;
    for (auto const &[_, ingredient] : allergenAndIngredient) {
      if (!first) {
        std::cout << ",";
      }
      std::cout << ingredient;
      first = false;
    }
    std::cout << "\n";
  }
}
} // namespace AoC

#include "Main.inl"
