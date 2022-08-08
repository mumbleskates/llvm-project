//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <variant>

// template <class... Types> class variant;

// template<class... Types>
//   constexpr std::common_comparison_category_t<
//     std::compare_three_way_result_t<Types>...>
//   operator<=>(const variant<Types...>& t, const variant<Types...>& u);

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <cassert>
#include <type_traits>
#include <utility>
#include <variant>

#include "test_macros.h"
#include "test_comparisons.h"

#ifndef TEST_HAS_NO_EXCEPTIONS
struct MakeEmptyT {
  MakeEmptyT() = default;
  MakeEmptyT(MakeEmptyT &&) { throw 42; }
  MakeEmptyT &operator=(MakeEmptyT &&) { throw 42; }
};
inline bool operator==(const MakeEmptyT &, const MakeEmptyT &) {
  assert(false);
  return false;
}
inline std::weak_ordering operator<=>(const MakeEmptyT&, const MakeEmptyT&) {
  assert(false);
  return std::weak_ordering::equivalent;
}

template <class Variant> void makeEmpty(Variant &v) {
  Variant v2(std::in_place_type<MakeEmptyT>);
  try {
    v = std::move(v2);
    assert(false);
  } catch (...) {
    assert(v.valueless_by_exception());
  }
}
#endif // TEST_HAS_NO_EXCEPTIONS

template <class Var, class Order>
constexpr bool test_three_way_val(const Var &l, const Var &r, Order order) {
  static_assert(std::is_same_v<decltype(l <=> r), Order>);
  return (l <=> r) == order;
}

template <class T1, class T2>
void test_with_types() {
  using V = std::variant<T1, T2>;
  using Order = std::compare_three_way_result_t<V>;
  { // same index, same value
    constexpr V v1(std::in_place_index<0>, T1{1});
    constexpr V v2(std::in_place_index<0>, T1{1});
    static_assert(testOrder(v1, v2, Order(std::strong_ordering::equivalent)));
  }
  { // same index, value < other_value
    constexpr V v1(std::in_place_index<0>, T1{0});
    constexpr V v2(std::in_place_index<0>, T1{1});
    static_assert(testOrder(v1, v2, Order(std::strong_ordering::less)));
  }
  { // same index, value > other_value
    constexpr V v1(std::in_place_index<0>, T1{1});
    constexpr V v2(std::in_place_index<0>, T1{0});
    static_assert(testOrder(v1, v2, Order(std::strong_ordering::greater)));
  }
  { // LHS.index() < RHS.index()
    constexpr V v1(std::in_place_index<0>, T1{0});
    constexpr V v2(std::in_place_index<1>, T2{0});
    static_assert(testOrder(v1, v2, Order(std::strong_ordering::less)));
  }
  { // LHS.index() > RHS.index()
    constexpr V v1(std::in_place_index<1>, T2{0});
    constexpr V v2(std::in_place_index<0>, T1{0});
    static_assert(testOrder(v1, v2, Order(std::strong_ordering::greater)));
  }
}

void test_three_way() {
  {
    using V = std::variant<int, double>;
    ASSERT_SAME_TYPE(std::partial_ordering, std::compare_three_way_result_t<V>);
    test_with_types<int, double>();
  }
  {
    using V = std::variant<int, long>;
    ASSERT_SAME_TYPE(std::strong_ordering, std::compare_three_way_result_t<V>);
    test_with_types<int, long>();
  }

#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    using V = std::variant<int, MakeEmptyT>;
    V v1;
    V v2;
    makeEmpty(v2);
    assert(testOrder(v1, v2, std::weak_ordering::greater));
  }
  {
    using V = std::variant<int, MakeEmptyT>;
    V v1;
    makeEmpty(v1);
    V v2;
    assert(testOrder(v1, v2, std::weak_ordering::less));
  }
  {
    using V = std::variant<int, MakeEmptyT>;
    V v1;
    makeEmpty(v1);
    V v2;
    makeEmpty(v2);
    assert(testOrder(v1, v2, std::weak_ordering::equivalent));
  }
#endif // TEST_HAS_NO_EXCEPTIONS
}

int main(int, char**) {
  test_three_way();

  return 0;
}
