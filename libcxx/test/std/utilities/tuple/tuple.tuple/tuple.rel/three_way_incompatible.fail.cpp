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
//   auto
//   operator<=>(const tuple<TTypes...>& t, const tuple<UTypes...>& u);

// UNSUPPORTED: c++03, c++11, c++14, c++17, libcpp-no-concepts

#include <tuple>

#include "test_macros.h"

int main(int, char**)
{
  static_cast<void>(std::tuple<int>(1) <=> std::tuple<int, long>(1, 2)); // expected-error {{invalid operands to binary expression}}
  static_cast<void>(std::tuple<int, long>(1, 2) <=> std::tuple<int>(1)); // expected-error {{invalid operands to binary expression}}
  return 0;
}
