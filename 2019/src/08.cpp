#include "ArgumentParser.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

constexpr int Width = 25;
constexpr int Height = 6;

using Layer = std::vector<std::vector<char>>;

std::optional<Layer> readLayer(std::istream &stream) {
  Layer layer(Height, std::vector<char>(Width, '\0'));
  for (int i = 0; i < Height; i++) {
    for (int j = 0; j < Width; j++) {
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

int count(const Layer &layer, char digit) {
  int total = 0;
  for (const auto &row : layer) {
    total += std::count(std::begin(row), std::end(row), digit);
  }
  return total;
}

Layer mergeLayers(const std::vector<Layer> &layers) {
  Layer layer(Height, std::vector<char>(Width, '2'));
  for (int i = 0; i < Height; i++) {
    for (int j = 0; j < Width; j++) {
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

int main(int argc, char **argv) {
  ArgumentParser argumentParser;
  argumentParser.parseArguments(argc, argv);
  const auto path = argumentParser.getPath();
  const auto part = argumentParser.getAdditionalArgument(0);
  std::ifstream stream(path);
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
    int bestLayerZeros = std::numeric_limits<int>::max();
    for (std::size_t i = 0; i < layers.size(); i++) {
      const auto thisLayerZeros = count(layers[i], '0');
      if (thisLayerZeros < bestLayerZeros) {
        bestLayer = i;
        bestLayerZeros = thisLayerZeros;
      }
    }
    std::cout << count(layers[bestLayer], '1') * count(layers[bestLayer], '2') << '\n';
  } else if (part == 2) {
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
  } else {
    throw std::invalid_argument("Part should be 1 or 2.");
  }
  return 0;
}
