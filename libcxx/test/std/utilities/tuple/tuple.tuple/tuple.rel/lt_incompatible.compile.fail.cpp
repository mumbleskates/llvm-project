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
//   operator<(const tuple<TTypes...>& t, const tuple<UTypes...>& u);

// UNSUPPORTED: c++03

#include <tuple>

static_cast<void>(std::tuple<int>(1) < std::tuple<int, long>(1, 2));
// c++20 onwards
// expected-error@-2 0-1 {{invalid operands to binary expression}}
static_cast<void>(std::tuple<int, long>(1, 2) < std::tuple<int>(1));
// c++20 onwards
// expected-error@-2 0-1 {{invalid operands to binary expression}}

// c++11, c++14, c++17
// expected-error-re@tuple:* 0-2 {{static_assert failed{{.*}} "Can't compare tuples of different sizes"}}
// expected-error-re@__tuple:* 0-1 {{static_assert failed{{.*}} "tuple_element index out of range"}}
// expected-error@tuple:* 0-1 {{no viable conversion from}}

// c++14, c++17
// expected-error@tuple:* 0-1 {{no matching function for call to 'get'}}
