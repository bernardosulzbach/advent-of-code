#pragma once

#include <cassert>
#include <stack>
#include <vector>

namespace AoC {
template <typename T> auto extractFront(std::deque<T> &d) noexcept {
  assert(!d.empty());
  auto front = d.front();
  d.pop_front();
  return front;
}

template <typename T> auto extractTop(std::stack<T> &t) noexcept {
  assert(!t.empty());
  auto top = t.top();
  t.pop();
  return top;
}

template <typename T> auto extractBack(std::vector<T> &v) noexcept {
  assert(!v.empty());
  auto back = v.back();
  v.pop_back();
  return back;
}
} // namespace AoC
