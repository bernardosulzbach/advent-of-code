#pragma once

#include <type_traits>

namespace AoC {
template <class T>
concept Integral = std::is_integral_v<T>;
template <class T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;
template <class T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;
} // namespace AoC