#pragma once

#include "Concepts.hpp"

#include <boost/numeric/conversion/cast.hpp>

#include <type_traits>

namespace AoC {
using Checked = std::true_type;
using Unchecked = std::false_type;

/**
 * Checks if the cast preserves the value before performing it.
 */
template <Integral Target, Integral Source> constexpr Target checkedCast(Source const source) {
  return boost::numeric_cast<Target>(source);
}

/**
 * Casts an integral to its unsigned version.
 *
 * If checked, checks if the cast preserves the value before performing it.
 */
template <typename IsChecked, Integral Source>
constexpr std::make_unsigned_t<Source>
unsignedCast(Source const source) noexcept(std::is_same_v<IsChecked, Unchecked>) {
  if constexpr (std::is_same_v<IsChecked, Checked>) {
    return checkedCast<std::make_unsigned_t<Source>>(source);
  } else {
    return static_cast<std::make_unsigned_t<Source>>(source);
  }
}

/**
 * Casts an integral to its signed version.
 *
 * If checked, checks if the cast preserves the value before performing it.
 */
template <typename IsChecked, Integral Source>
constexpr std::make_signed_t<Source> signedCast(Source const source) noexcept(std::is_same_v<IsChecked, Unchecked>) {
  if constexpr (std::is_same_v<IsChecked, Checked>) {
    return checkedCast<std::make_signed_t<Source>>(source);
  } else {
    return static_cast<std::make_signed_t<Source>>(source);
  }
}
} // namespace AoC
