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
//   operator>=(const tuple<TTypes...>& t, const tuple<UTypes...>& u);

// UNSUPPORTED: c++03

#include <tuple>

#include "test_macros.h"

int main(int, char**)
{
  static_cast<void>(std::tuple<int>(1) >= std::tuple<int, long>(1, 2)); // expected-note {{requested here}}
  static_cast<void>(std::tuple<int, long>(1, 2) >= std::tuple<int>(1)); // expected-note {{requested here}}
  // Always
  // expected-error-re@tuple:* 2 {{static_assert failed{{.*}} "Can't compare tuples of different sizes"}}

  // c++11, c++14, c++17
  // expected-error-re@__tuple:* 0-1 {{static_assert failed{{.*}} "tuple_element index out of range"}}
  // expected-error@tuple:* 0-1 {{no viable conversion from}}

  // c++14, c++17
  // expected-error@tuple:* 0-1 {{no matching function for call to 'get'}}

  // c++20 onwards
  // expected-error@tuple:* 0-2 {{pack expansion contains parameter packs '_Tp' and '_Up' that have different lengths}}
  return 0;
}
