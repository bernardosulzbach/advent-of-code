#include "ArgumentParser.hpp"
#include "Math.hpp"

#include <algorithm>

#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <type_traits>

namespace AoC {
static constexpr Size Width = 25;
static constexpr Size Height = 6;

using Layer = std::vector<std::vector<char>>;

std::optional<Layer> readLayer(std::istream &stream) {
  Layer layer(Height, std::vector<char>(Width, '\0'));
  for (Size i = 0; i < Height; i++) {
    for (Size j = 0; j < Width; j++) {
      stream >> layer[i][j];
      if (!stream.good()) {
        if (i == 0 && j == 0) {
          return std::nullopt;
        }
        throw std::runtime_error("Failed to read while reading a layer.");
      }
    }
  }
  return layer;
}

[[nodiscard]] Size count(const Layer &layer, char digit) noexcept {
  return std::accumulate(std::begin(layer), std::end(layer), Size{},
                         [digit](auto const count, auto const &row) noexcept {
                           return count + unsignedCast<Unchecked>(std::count(std::begin(row), std::end(row), digit));
                         });
}

Layer mergeLayers(const std::vector<Layer> &layers) {
  Layer layer(Height, std::vector<char>(Width, '2'));
  for (Size i = 0; i < Height; i++) {
    for (Size j = 0; j < Width; j++) {
      for (const auto &transmittedLayer : layers) {
        if (layer[i][j] == '0' || layer[i][j] == '1') {
          break;
        }
        layer[i][j] = transmittedLayer[i][j];
      }
    }
  }
  return layer;
}

void main(std::istream &stream, U32 const part) {
  std::vector<Layer> layers;
  while (true) {
    const auto layer = readLayer(stream);
    if (!layer) {
      break;
    }
    layers.push_back(layer.value());
  }
  if (part == 1) {
    std::size_t bestLayer = 0;
    auto bestLayerZeros = std::numeric_limits<Size>::max();
    for (std::size_t i = 0; i < layers.size(); i++) {
      const auto thisLayerZeros = count(layers[i], '0');
      if (thisLayerZeros < bestLayerZeros) {
        bestLayer = i;
        bestLayerZeros = thisLayerZeros;
      }
    }
    std::cout << count(layers[bestLayer], '1') * count(layers[bestLayer], '2') << '\n';
  } else {
    const auto mergedLayer = mergeLayers(layers);
    for (const auto &row : mergedLayer) {
      for (const auto &pixel : row) {
        if (pixel == '0') {
          std::cout << " ";
        } else if (pixel == '1') {
          std::cout << "█";
        } else {
          throw std::invalid_argument("Pixel cannot be other than 0 or 1 in the merged image.");
        }
      }
      std::cout << '\n';
    }
  }
}
} // namespace AoC

#include "Main.inl"
