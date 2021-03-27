#include "ArgumentParser.hpp"
#include "Casts.hpp"
#include "Containers.hpp"
#include "IO.hpp"
#include "Text.hpp"
#include "Types.hpp"

#include <algorithm>

#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace AoC {
using CardType = U32;

struct [[nodiscard]] Decks {
  std::array<std::deque<CardType>, 2> decks;

  [[nodiscard]] auto &operator[](Size const i) &noexcept {
    return decks[i];
  }

  [[nodiscard]] auto const &operator[](Size const i) const &noexcept {
    return decks[i];
  }

  [[nodiscard]] constexpr auto operator<=>(Decks const &rhs) const noexcept = default;

  [[nodiscard]] std::array<CardType, 2> takeTopCards() noexcept {
    return {extractFront(decks[0]), extractFront(decks[1])};
  }

  [[nodiscard]] U64 evaluateWinnerScore() const noexcept {
    auto const &winnerDeck = decks[0].empty() ? decks[1] : decks[0];
    U64 score = 0;
    for (Size i = 0; i < winnerDeck.size(); ++i) {
      score += (winnerDeck.size() - i) * winnerDeck[i];
    }
    return score;
  }

  [[nodiscard]] bool hasEmptyDeck() const noexcept {
    return std::ranges::any_of(decks, [](auto const &deck) noexcept { return deck.empty(); });
  }

  [[nodiscard]] Decks copyFirst(Size const i, Size const j) const noexcept {
    auto copy = *this;
    copy[0].erase(std::begin(copy[0]) + signedCast<Unchecked>(i), std::end(copy[0]));
    copy[1].erase(std::begin(copy[1]) + signedCast<Unchecked>(j), std::end(copy[1]));
    return copy;
  }

  void insertWinningCards(Size const player, std::array<CardType, 2> const cards) noexcept {
    decks[player].insert(std::end(decks[player]), std::begin(cards), std::end(cards));
  }
};

[[nodiscard]] Decks play(Decks decks, bool recursive) {
  std::set<Decks> seenDecks;
  while (true) {
    if (seenDecks.contains(decks)) {
      decks[0].insert(std::end(decks[0]), std::begin(decks[1]), std::end(decks[1]));
      decks[1].clear();
      break;
    }
    seenDecks.insert(decks);
    if (decks.hasEmptyDeck()) {
      break;
    }
    Size winner = 0;
    auto cards = decks.takeTopCards();
    if (recursive && cards[0] <= decks[0].size() && cards[1] <= decks[1].size()) { // Recurse.
      auto const subGameFinalDecks = play(decks.copyFirst(cards[0], cards[1]), recursive);
      if (subGameFinalDecks[0].empty()) {
        winner = 1;
      }
    } else { // Not enough cards, higher value wins.
      if (cards[1] > cards[0]) {
        winner = 1;
      }
    }
    if (winner == 1) {
      std::swap(cards[0], cards[1]);
    }
    decks.insertWinningCards(winner, cards);
  }
  return decks;
}

void main(std::istream &stream, U32 const part) {
  Decks decks;
  {
    Size readingInto;
    for (auto const line : readLines(stream)) {
      if (line.starts_with("Player")) {
        readingInto = parseValue<decltype(readingInto)>(line.substr(7, 1)) - 1;
      } else if (!line.empty()) {
        decks[readingInto].push_back(parseValue<CardType>(line));
      }
    }
  }
  if (part == 1) {
    decks = play(decks, false);
  } else {
    decks = play(decks, true);
  }
  std::cout << decks.evaluateWinnerScore() << "\n";
}
} // namespace AoC

#include "Main.inl"
