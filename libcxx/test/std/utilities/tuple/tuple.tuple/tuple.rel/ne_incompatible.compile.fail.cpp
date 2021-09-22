//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <tuple>

// template <class... Types> class tuple;

// template<class... TTypes, class... UTypes>
//   bool
//   operator!=(const tuple<TTypes...>& t, const tuple<UTypes...>& u);

// UNSUPPORTED: c++03

#include <tuple>

void test() {
  static_cast<void>(std::tuple<int>(1) != std::tuple<int, long>(1, 2)); // expected-note {{requested here}}
  // expected-error-re@tuple:* {{static_assert failed{{.*}} "Can't compare tuples of different sizes"}}

  static_cast<void>(std::tuple<int, long>(1, 2) != std::tuple<int>(1)); // expected-note {{requested here}}
  // expected-error-re@tuple:* {{static_assert failed{{.*}} "Can't compare tuples of different sizes"}}
  // expected-error-re@__tuple:* {{static_assert failed{{.*}} "tuple_element index out of range"}}
  // expected-error@tuple:* {{no viable conversion from}}
}
