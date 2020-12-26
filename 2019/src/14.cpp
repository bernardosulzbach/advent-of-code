#include "ArgumentParser.hpp"
#include "Body.hpp"
#include "Cycle.hpp"
#include "DirectedGraph.hpp"
#include "Intcode.hpp"
#include "Math.hpp"
#include "Point.hpp"
#include "Screen.hpp"
#include "Text.hpp"
#include "Vector.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Term {
  U32 coefficient{};
  U32 index{};
};

std::ostream &operator<<(std::ostream &stream, const Term &term) {
  stream << term.coefficient << ' ' << term.index;
  return stream;
}

struct Reaction {
  std::vector<Term> inputs{};
  Term output{};
};

std::ostream &operator<<(std::ostream &stream, const Reaction &reaction) {
  auto first = true;
  for (const auto input : reaction.inputs) {
    if (!first) {
      stream << ", ";
    }
    stream << input;
    first = false;
  }
  stream << " => ";
  stream << reaction.output;
  return stream;
}

Term readTerm(const std::string &string, std::unordered_map<std::string, U32> &dictionary) {
  Term term{};
  std::stringstream termStream(string);
  termStream >> term.coefficient;
  std::string name;
  termStream >> name;
  if (dictionary.count(name) == 0) {
    const auto index = static_cast<U32>(dictionary.size());
    dictionary[name] = index;
  }
  term.index = dictionary[name];
  return term;
}

std::vector<Reaction> readReactionsFromFile(const std::string &path, std::unordered_map<std::string, U32> &dictionary) {
  std::vector<Reaction> reactions;
  std::ifstream stream(path);
  std::string line;
  while (stream && std::getline(stream, line) && !line.empty()) {
    const auto sides = splitAt(line, " => ");
    std::vector<Term> inputs;
    for (const auto &termString : splitAt(sides[0], ", ")) {
      inputs.push_back(readTerm(termString, dictionary));
    }
    reactions.push_back(Reaction{inputs, readTerm(sides[1], dictionary)});
  }
  return reactions;
}

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  const auto part = argumentParser.getAdditionalArgument(0);
  if (part != 1 && part != 2) {
    throw std::invalid_argument("Part should be 1 or 2.");
  }

  std::unordered_map<std::string, U32> dictionary;
  const auto reactions = readReactionsFromFile(path, dictionary);
  std::vector<std::string> reverseDictionary(dictionary.size());
  for (const auto &entry : dictionary) {
    reverseDictionary[entry.second] = entry.first;
  }

  std::vector<U32> reactionToMakeChemical(dictionary.size());
  for (std::size_t i = 0; i < reactions.size(); i++) {
    reactionToMakeChemical[reactions[i].output.index] = i;
  }

  DirectedGraph directedGraph(dictionary.size());
  for (const auto &reaction : reactions) {
    for (const auto &input : reaction.inputs) {
      directedGraph.addEdge(reaction.output.index, input.index);
    }
  }
  const auto ordering = directedGraph.topologicalSort();

  const auto determineOreRequiredToMakeFuel = [&dictionary, &reactions, &reactionToMakeChemical, &ordering](U64 fuel) {
    std::vector<U64> demand(dictionary.size());
    demand[dictionary["FUEL"]] = fuel;
    for (const auto &index : ordering) {
      if (index == dictionary["ORE"]) {
        break;
      }
      const auto reaction = reactions[reactionToMakeChemical[index]];
      const auto productivity = reaction.output.coefficient;
      const auto multiplier = (demand[reaction.output.index] + productivity - 1) / productivity;
      for (const auto input : reaction.inputs) {
        demand[input.index] += multiplier * input.coefficient;
      }
    }
    return demand[dictionary["ORE"]];
  };

  if (part == 1) {
    std::cout << determineOreRequiredToMakeFuel(1) << '\n';
  } else {
    constexpr U64 FuelLimit = 1000000000000ULL;
    U64 a = 1;
    U64 b = FuelLimit;
    while (a + 1 < b) {
      const auto m = a + (b - a) / 2;
      if (determineOreRequiredToMakeFuel(m) <= FuelLimit) {
        a = m;
      } else {
        b = m;
      }
    }
    assert(determineOreRequiredToMakeFuel(a) <= FuelLimit);
    std::cout << a << '\n';
  }

  return 0;
}
