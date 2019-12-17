#include "DirectedGraph.hpp"

DirectedGraph::DirectedGraph(std::size_t size) {
  adjacency.resize(size);
}

void DirectedGraph::addEdge(std::size_t v, std::size_t w) {
  adjacency[v].push_back(w);
}

void DirectedGraph::topologicalSortUtil(std::size_t i, std::vector<bool> &visited, std::stack<std::size_t> &results) {
  visited[i] = true;
  for (auto &j : adjacency[i]) {
    if (!visited[j]) {
      topologicalSortUtil(j, visited, results);
    }
  }
  results.push(i);
}

std::vector<std::size_t> DirectedGraph::topologicalSort() {
  std::stack<std::size_t> result;
  std::vector<bool> visited(adjacency.size());
  for (std::size_t i = 0; i < adjacency.size(); i++) {
    if (visited[i] == false) {
      topologicalSortUtil(i, visited, result);
    }
  }
  std::vector<std::size_t> ordering;
  while (!result.empty()) {
    ordering.push_back(result.top());
    result.pop();
  }
  return ordering;
}
