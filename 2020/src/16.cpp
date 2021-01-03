#include "ArgumentParser.hpp"
#include "IO.hpp"
#include "Interval.hpp"
#include "Text.hpp"
#include "Types.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <unordered_map>

using ValueType = U64; // Needed for the product in part 2.

struct Ticket {
  std::vector<ValueType> values;

  Ticket() = default;

  explicit Ticket(std::string_view const sv) {
    for (auto const &valueString : splitAt(sv, ",")) {
      std::stringstream stringStream(valueString);
      values.push_back(AoC::readValue<ValueType>(stringStream));
    }
  }
};

struct Field {
  std::string name;
  std::vector<AoC::Interval<ValueType>> intervals;
};

template <typename Stream> std::vector<Field> readFields(Stream &stream) noexcept {
  std::vector<Field> fields;
  std::string line;
  while (std::getline(stream, line) && !line.empty()) {
    auto const colonPosition = line.find(':');
    assert(colonPosition != std::string::npos);
    auto const name = line.substr(0, colonPosition);
    std::vector<AoC::Interval<ValueType>> intervals;
    std::stringstream stringStream(line.substr(colonPosition + 1));
    auto const pushInterval = [&intervals, &stringStream]() {
      ValueType start;
      ValueType finish;
      (stringStream >> start).ignore() >> finish;
      assert(finish != std::numeric_limits<decltype(finish)>::max());
      intervals.emplace_back(start, finish + 1);
    };
    pushInterval();
    stringStream.ignore(4);
    pushInterval();
    fields.push_back({name, intervals});
  }
  return fields;
}

int main(int argc, char **argv) {
  try {
    ArgumentParser argumentParser;
    argumentParser.parseArguments(argc, argv);
    auto const part = argumentParser.getAdditionalArgument(0);
    auto stream = std::ifstream(argumentParser.getPath());
    auto const fields = readFields(stream);
    std::vector<AoC::Interval<ValueType>> mergedFieldIntervals;
    std::ranges::for_each(fields, [&mergedFieldIntervals](auto const &field) {
      auto const atEnd = std::end(mergedFieldIntervals);
      mergedFieldIntervals.insert(atEnd, std::begin(field.intervals), std::end(field.intervals));
    });
    std::ranges::sort(mergedFieldIntervals);
    AoC::mergeIntervals(mergedFieldIntervals);
    Ticket yourTicket;
    std::vector<Ticket> nearbyTickets;
    {
      std::string line;
      while (std::getline(stream, line) && !line.empty()) {
        if (std::isdigit(line.front())) {
          yourTicket = Ticket(line);
        }
      }
      while (std::getline(stream, line)) {
        if (std::isdigit(line.front())) {
          nearbyTickets.emplace_back(line);
        }
      }
    }
    auto isValidFieldValue = [&mergedFieldIntervals](auto const fieldValue) noexcept {
      return AoC::inIntervalUnion(mergedFieldIntervals, fieldValue);
    };
    auto const isInvalidTicket = [&isValidFieldValue](auto const &ticket) noexcept {
      return !std::ranges::all_of(ticket.values, isValidFieldValue);
    };
    if (part == 1) {
      ValueType ticketScanningErrorRate{};
      for (auto const &ticket : nearbyTickets) {
        for (auto const value : ticket.values) {
          if (!isValidFieldValue(value)) {
            ticketScanningErrorRate += value;
          }
        }
      }
      std::cout << ticketScanningErrorRate << '\n';
    } else {
      std::erase_if(nearbyTickets, isInvalidTicket);
      std::vector<std::vector<bool>> canBe(fields.size(), std::vector<bool>(fields.size(), true));
      for (auto const &nearbyTicket : nearbyTickets) {
        assert(nearbyTicket.values.size() == fields.size());
        for (std::size_t i = 0; i < nearbyTicket.values.size(); i++) {
          for (std::size_t j = 0; j < fields.size(); j++) {
            if (!canBe[i][j]) {
              continue;
            }
            canBe[i][j] = AoC::inIntervalUnion(fields[j].intervals, nearbyTicket.values[i]);
          }
        }
      }
      auto const Unknown = std::numeric_limits<std::size_t>::max();
      std::vector<std::size_t> is(fields.size(), Unknown);
      for (std::size_t discovered = 0; discovered < fields.size(); discovered++) {
        for (std::size_t i = 0; i < fields.size(); i++) {
          if (is[i] != Unknown) {
            continue;
          }
          auto const itJ = std::find(std::begin(canBe[i]), std::end(canBe[i]), true);
          assert(itJ != std::end(canBe[i]));
          if (std::find(std::next(itJ), std::end(canBe[i]), true) == std::end(canBe[i])) {
            auto const j = std::distance(std::begin(canBe[i]), itJ);
            is[i] = j;
            for (std::size_t k = 0; k < fields.size(); k++) {
              if (k != i) {
                canBe[k][j] = false;
              }
            }
            break;
          }
        }
      }
      assert(std::ranges::count(is, Unknown) == 0);
      ValueType total = 1;
      for (std::size_t i = 0; i < fields.size(); i++) {
        if (fields[is[i]].name.starts_with("departure")) {
          total *= yourTicket.values[i];
        }
      }
      std::cout << total << '\n';
    }
  } catch (const std::exception &exception) {
    std::cout << "Threw: " << exception.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
