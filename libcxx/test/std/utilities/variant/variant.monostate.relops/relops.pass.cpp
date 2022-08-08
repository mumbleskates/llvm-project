//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <variant>

// constexpr bool operator<(monostate, monostate) noexcept { return false; }
// constexpr bool operator>(monostate, monostate) noexcept { return false; }
// constexpr bool operator<=(monostate, monostate) noexcept { return true; }
// constexpr bool operator>=(monostate, monostate) noexcept { return true; }
// constexpr bool operator==(monostate, monostate) noexcept { return true; }
// constexpr bool operator!=(monostate, monostate) noexcept { return false; }

#include "test_macros.h"
#include <cassert>
#include <type_traits>
#include <variant>

#if TEST_STD_VER > 17
#include <compare>
#endif // TEST_STD_VER > 17

int main(int, char**) {
  using M = std::monostate;
  constexpr M m1{};
  constexpr M m2{};
  {
    static_assert((m1 < m2) == false, "");
    ASSERT_NOEXCEPT(m1 < m2);
  }
  {
    static_assert((m1 > m2) == false, "");
    ASSERT_NOEXCEPT(m1 > m2);
  }
  {
    static_assert((m1 <= m2) == true, "");
    ASSERT_NOEXCEPT(m1 <= m2);
  }
  {
    static_assert((m1 >= m2) == true, "");
    ASSERT_NOEXCEPT(m1 >= m2);
  }
  {
    static_assert((m1 == m2) == true, "");
    ASSERT_NOEXCEPT(m1 == m2);
  }
  {
    static_assert((m1 != m2) == false, "");
    ASSERT_NOEXCEPT(m1 != m2);
  }
#if TEST_STD_VER > 17
  {
    static_assert((m1 <=> m2) == std::strong_ordering::equal);
    ASSERT_SAME_TYPE(decltype(m1 <=> m2), std::strong_ordering);
    ASSERT_NOEXCEPT(m1 <=> m2);
  }
#endif // TEST_STD_VER > 17

  return 0;
}
