#pragma once

#include <iostream>
#include <list>
#include <stack>
#include <vector>

class DirectedGraph {
  std::vector<std::list<std::size_t>> adjacency;

  void topologicalSortUtil(std::size_t i, std::vector<bool> &visited, std::stack<std::size_t> &results);

public:
  explicit DirectedGraph(std::size_t size);

  void addEdge(std::size_t v, std::size_t w);

  std::vector<std::size_t> topologicalSort();
};
