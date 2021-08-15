//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <utility>

// template <class T1, class T2> struct pair

// template <class T1, class T2> bool operator<=>(const pair<T1,T2>&, const pair<T1,T2>&);

// UNSUPPORTED: c++03, c++11, c++14, c++17, libcpp-no-concepts

#include <cassert>
#include <limits>
#include <type_traits>
#include <utility>

#include "test_macros.h"

int main(int, char**) {
  {
    // Pairs of types that both have strong ordering should compare with strong ordering.
    static_assert((std::make_pair(1, 1) <=> std::make_pair(1, 2)) < 0, "");
    static_assert((std::make_pair(2, 1) <=> std::make_pair(1, 2)) > 0, "");
    auto same = std::make_pair(0, 0) <=> std::make_pair(0, 0);
    assert(same == 0);
    ASSERT_SAME_TYPE(std::strong_ordering, decltype(same));
  }
  {
    // Pairs of int and a type with no spaceship operator should compare with weak ordering.
    struct NoSpaceship {
      int value;
      constexpr bool operator==(const NoSpaceship&) const = default;
      constexpr bool operator<(const NoSpaceship& other) const { return value < other.value; }
    };
    static_assert((std::make_pair(1, NoSpaceship{1}) <=> std::make_pair(1, NoSpaceship{2})) < 0, "");
    static_assert((std::make_pair(2, NoSpaceship{1}) <=> std::make_pair(1, NoSpaceship{2})) > 0, "");
    auto same = std::make_pair(0, NoSpaceship{0}) <=> std::make_pair(0, NoSpaceship{0});
    assert(same == 0);
    ASSERT_SAME_TYPE(std::weak_ordering, decltype(same));
  }
  {
    // Pairs of int (strongly ordered) and double (partially ordered) should compare with partial ordering.
    static_assert((std::make_pair(1, 1.0) <=> std::make_pair(1, 2.0)) < 0, "");
    static_assert((std::make_pair(2, 1.0) <=> std::make_pair(1, 1.0)) > 0, "");
    static_assert((std::make_pair(std::numeric_limits<double>::quiet_NaN(), 1)
                   <=> std::make_pair(std::numeric_limits<double>::quiet_NaN(), 2))
                  == std::partial_ordering::unordered, "");
    auto same = std::make_pair(0, 0.0) <=> std::make_pair(0, 0.0);
    assert(same == 0);
    ASSERT_SAME_TYPE(std::partial_ordering, decltype(same));
  }

  return 0;
}
