#pragma once

#include "Debugging.hpp"
#include "Hash.hpp"
#include "IO.hpp"
#include "Types.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <vector>

namespace AoC {
// A formal language production rule of the form S -> ...
class Production {
public:
  template <typename StringRange>
  explicit Production(std::string_view const productionName, StringRange const &range) noexcept
      : name(productionName), products(std::ranges::begin(range), std::ranges::end(range)) {
  }

  friend std::ostream &operator<<(std::ostream &stream, Production const &production) noexcept {
    stream << production.name << " → ";
    AoC::print(stream, production.products, " ", false);
    return stream;
  }

  [[nodiscard]] std::string const &getName() const noexcept {
    return name;
  }

  [[nodiscard]] std::vector<std::string> const &getProducts() const noexcept {
    return products;
  }

private:
  std::string name;
  std::vector<std::string> products;
};

class ContextFreeGrammar {
public:
  template <typename ProductionRange>
  explicit ContextFreeGrammar(ProductionRange const &range) noexcept
      : productions(std::ranges::begin(range), std::ranges::end(range)) {
    for (Size i = 0; i < getProductions().size(); i++) {
      productionIndexesByName[getProductions()[i].getName()].push_back(i);
    }
  }

  [[nodiscard]] std::vector<Production> const &getProductions() const noexcept {
    return productions;
  }

  [[nodiscard]] std::vector<Size> const &getProductionIndexesForName(std::string const &name) const noexcept {
    assert(isNonTerminal(name));
    return productionIndexesByName.at(name);
  }

  [[nodiscard]] bool isNonTerminal(std::string const &string) const noexcept {
    return productionIndexesByName.contains(string);
  }

private:
  std::unordered_map<std::string, std::vector<Size>> productionIndexesByName;
  std::vector<Production> productions;
};

template <DebuggingInformation debuggingInformation = DebuggingInformation::Off> class ContextFreeGrammarRecognizer {
public:
  class State {
  public:
    State(ContextFreeGrammar const &contextFreeGrammar, Size const stateProductionIndex,
          Size const stateOriginPosition) noexcept
        : grammar(&contextFreeGrammar), productionIndex(stateProductionIndex), originPosition(stateOriginPosition) {
    }

    [[nodiscard]] Size getProductionIndex() const noexcept {
      return productionIndex;
    }

    [[nodiscard]] std::string_view getProductionName() const noexcept {
      return grammar->getProductions()[getProductionIndex()].getName();
    }

    [[nodiscard]] std::string const &nextElement() const noexcept {
      assert(!isComplete());
      return grammar->getProductions()[getProductionIndex()].getProducts()[currentPosition];
    }

    [[nodiscard]] State advance() const noexcept {
      auto advancedCopy = *this;
      advancedCopy.currentPosition++;
      return advancedCopy;
    }

    [[nodiscard]] bool isComplete() const noexcept {
      return currentPosition == grammar->getProductions()[getProductionIndex()].getProducts().size();
    }

    [[nodiscard]] Size getOriginPosition() const noexcept {
      return originPosition;
    }

    [[nodiscard]] constexpr auto operator<=>(State const &rhs) const noexcept = default;

    friend std::ostream &operator<<(std::ostream &stream, State const &state) noexcept {
      auto const &productions = state.grammar->getProductions();
      auto const &production = productions[state.getProductionIndex()];
      stream << production.getName() << " →";
      for (Size i = 0; i <= production.getProducts().size(); i++) {
        if (i == state.currentPosition) {
          stream << " ·";
        }
        if (i < production.getProducts().size()) {
          stream << " " << production.getProducts()[i];
        }
      }
      return stream;
    }

  private:
    ContextFreeGrammar const *grammar;
    Size productionIndex;
    Size currentPosition = 0;
    Size originPosition;
  };

  explicit ContextFreeGrammarRecognizer(ContextFreeGrammar const &contextFreeGrammar,
                                        Size const startingProductionIndex) noexcept
      : grammar(&contextFreeGrammar), startingProduction(startingProductionIndex) {
  }

  [[nodiscard]] bool isRecognized(std::vector<std::string> const &words) noexcept {
    states.clear();
    states.resize(words.size() + 1);
    states[0].push_back(State(*grammar, startingProduction, 0));
    for (Size k = 0; k < states.size(); k++) {
      for (Size i = 0; i < states[k].size(); i++) {
        auto const state = states[k][i];
        if (!state.isComplete()) {
          if (grammar->isNonTerminal(state.nextElement())) {
            predict(state, k);
          } else {
            scan(state, k, words);
          }
        } else {
          complete(state, k);
        }
      }
      if constexpr (debuggingInformation == DebuggingInformation::On) {
        std::cout << "States(" << k << "):\n";
        for (auto const state : states[k]) {
          std::cout << "  " << state << '\n';
        }
      }
    }
    return std::ranges::any_of(states.back(), [this](auto const state) {
      return state.getProductionIndex() == startingProduction && state.isComplete();
    });
  }

private:
  void addToSet(State const state, Size const k) noexcept {
    // Using a dedicated vector of unordered sets of states was substantially slower than just calling find().
    if (std::ranges::find(states[k], state) == std::ranges::end(states[k])) {
      states[k].push_back(state);
    }
  }

  void predict(State const state, Size const k) noexcept {
    assert(!state.isComplete());
    assert(!grammar->getProductionIndexesForName(state.nextElement()).empty());
    for (auto const productionIndex : grammar->getProductionIndexesForName(state.nextElement())) {
      addToSet(State(*grammar, productionIndex, k), k);
    }
  }

  void scan(State const state, Size const k, std::vector<std::string> const &words) noexcept {
    assert(!state.isComplete());
    if (k != words.size() &&
        state.nextElement() == words[k]) { // Wikipedia mentions PARTS-OF-SPEECH, this seems to be the right check.
      addToSet(state.advance(), k + 1);
    }
  }

  void complete(State const state, Size const k) noexcept {
    assert(state.isComplete());
    for (auto const otherState : states[state.getOriginPosition()]) {
      if (!otherState.isComplete() && otherState.nextElement() == state.getProductionName()) {
        addToSet(otherState.advance(), k);
      }
    }
  };

  void printStates(Size const index) const noexcept {
    print(std::cout, states[index], "\n");
  }

  ContextFreeGrammar const *grammar;
  Size startingProduction;
  std::vector<std::vector<State>> states;
};
} // namespace AoC
