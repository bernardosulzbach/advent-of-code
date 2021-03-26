#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Point.hpp"
#include "Text.hpp"

#include <algorithm>
#include <cassert>

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace AoC {
inline constexpr char const *ShinyGoldType = "shiny gold";

struct BagTypeAmount {
  int amount;
  std::string bagType;

  BagTypeAmount(int initialAmount, std::string initialBagType)
      : amount(initialAmount), bagType(std::move(initialBagType)) {
  }
};

using ContainsMap = std::unordered_map<std::string, std::vector<BagTypeAmount>>;

enum class Status { Unknown, True, False };
using StatusMap = std::unordered_map<std::string, Status>;

[[nodiscard]] std::vector<BagTypeAmount> getBagTypeAmounts(std::string const &listOfBagTypes) {
  std::vector<BagTypeAmount> bagTypeAmounts;
  std::stringstream stream(listOfBagTypes);
  std::string word;
  int amount;
  std::string bagType;
  while (stream >> word) {
    if (word.ends_with(',') || word.ends_with('.')) {
      bagTypeAmounts.emplace_back(amount, bagType);
      bagType.clear();
      continue;
    }
    if (std::isdigit(word[0])) {
      std::stringstream amountStream(word);
      amountStream >> amount;
      continue;
    }
    if (!bagType.empty()) {
      bagType += " ";
    }
    bagType += word;
  }
  assert(!bagTypeAmounts.empty());
  return bagTypeAmounts;
}

[[nodiscard]] bool findIfCanContainShinyGold(ContainsMap const &contains, StatusMap &canContainShinyGold,
                                             std::string const &bagType) {
  auto &status = canContainShinyGold[bagType];
  if (status == Status::Unknown) {
    auto const &bagContains = contains.at(bagType);
    assert(std::ranges::find_if(bagContains, [](auto const &bagTypeAmount) {
             return bagTypeAmount.bagType == ShinyGoldType;
           }) == std::ranges::end(bagContains));
    for (auto const &contained : bagContains) {
      if (findIfCanContainShinyGold(contains, canContainShinyGold, contained.bagType)) {
        status = Status::True;
        break;
      }
    }
    if (status == Status::Unknown) {
      status = Status::False;
    }
  }
  return status == Status::True;
}

using AmountContainedByMap = std::unordered_map<std::string, int>;
[[nodiscard]] int findBagsContainedBy(ContainsMap const &contains, AmountContainedByMap &amountContainedBy,
                                      std::string const &bagType) {
  {
    auto const it = amountContainedBy.find(bagType);
    if (it != amountContainedBy.end()) {
      return it->second;
    }
  }
  int bagsContainedByThisBag = 0;
  for (auto const &containedBagTypeAmount : contains.at(bagType)) {
    auto const containedByContainedBag =
        findBagsContainedBy(contains, amountContainedBy, containedBagTypeAmount.bagType);
    bagsContainedByThisBag += containedBagTypeAmount.amount * (1 + containedByContainedBag);
  }
  return amountContainedBy[bagType] = bagsContainedByThisBag;
}

void main(std::istream &stream, U32 const part) {
  ContainsMap contains;
  StatusMap canContainShinyGold;
  std::string line;
  while (std::getline(stream, line)) {
    auto const split = splitAt(line, " bags contain ");
    assert(split.size() == 2);
    auto const &left = split[0];
    auto &vector = contains[left];
    auto const &right = split[1];
    if (right == "no other bags.") {
      canContainShinyGold[left] = Status::False;
      continue;
    }
    canContainShinyGold[left] = Status::Unknown;
    for (auto const &contained : getBagTypeAmounts(right)) {
      if (contained.bagType == ShinyGoldType) {
        canContainShinyGold[left] = Status::True;
      }
      vector.push_back(contained);
    }
  }
  if (part == 1) {
    int count = 0;
    for (auto const &[bag, contents] : contains) {
      if (findIfCanContainShinyGold(contains, canContainShinyGold, bag)) {
        count++;
      }
    }
    std::cout << count << '\n';
  } else {
    AmountContainedByMap amountContainedBy;
    std::cout << findBagsContainedBy(contains, amountContainedBy, ShinyGoldType) << '\n';
  }
}
} // namespace AoC

#include "Main.inl"
