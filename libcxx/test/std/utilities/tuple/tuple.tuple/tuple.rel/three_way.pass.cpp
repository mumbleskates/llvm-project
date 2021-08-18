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

#include <cassert>
#include <compare>
#include <limits>
#include <tuple>

#include "test_macros.h"

// A custom three-way result type
struct CustomResult {
  friend constexpr bool operator==(int, const CustomResult&) noexcept { return true; }
  friend constexpr bool operator<(int, const CustomResult&) noexcept { return false; }
  friend constexpr bool operator<(const CustomResult&, int) noexcept { return false; }
};

int main(int, char**)
{
    // Empty tuple
    {
        typedef std::tuple<> T0;
        static_assert((T0() <=> T0()) == std::strong_ordering::equal, "");
        // No member types yields strong ordering (all are equal).
        ASSERT_SAME_TYPE(decltype(T0() <=> T0()), std::strong_ordering);
    }
    // Mixed types with integers, which compare strongly ordered
    {
        typedef std::tuple<long> T1;
        typedef std::tuple<short> T2;
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::strong_ordering);
        static_assert((T1(1) <=> T2(1)) == std::strong_ordering::equal, "");
        static_assert((T1(1) <=> T2(0)) == std::strong_ordering::greater, "");
        static_assert((T1(1) <=> T2(2)) == std::strong_ordering::less, "");
    }
    {
        typedef std::tuple<long, int> T1;
        typedef std::tuple<short, long> T2;
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::strong_ordering);
        static_assert((T1(1, 2) <=> T2(1, 2)) == std::strong_ordering::equal, "");
        static_assert((T1(1, 2) <=> T2(0, 2)) == std::strong_ordering::greater, "");
        static_assert((T1(1, 2) <=> T2(2, 2)) == std::strong_ordering::less, "");
        static_assert((T1(1, 2) <=> T2(1, 1)) == std::strong_ordering::greater, "");
        static_assert((T1(1, 2) <=> T2(1, 3)) == std::strong_ordering::less, "");
    }
    {
        typedef std::tuple<long, int, short> T1;
        typedef std::tuple<short, long, int> T2;
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::strong_ordering);
        static_assert((T1(1, 2, 3) <=> T2(1, 2, 3)) == std::strong_ordering::equal, "");
        static_assert((T1(1, 2, 3) <=> T2(0, 2, 3)) == std::strong_ordering::greater, "");
        static_assert((T1(1, 2, 3) <=> T2(2, 2, 3)) == std::strong_ordering::less, "");
        static_assert((T1(1, 2, 3) <=> T2(1, 1, 3)) == std::strong_ordering::greater, "");
        static_assert((T1(1, 2, 3) <=> T2(1, 3, 3)) == std::strong_ordering::less, "");
        static_assert((T1(1, 2, 3) <=> T2(1, 2, 2)) == std::strong_ordering::greater, "");
        static_assert((T1(1, 2, 3) <=> T2(1, 2, 4)) == std::strong_ordering::less, "");
    }
    // Mixed types with floating point, which compare partially ordered
    {
        typedef std::tuple<long> T1;
        typedef std::tuple<double> T2;
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::partial_ordering);
        static_assert((T1(1) <=> T2(1)) == std::partial_ordering::equivalent, "");
        static_assert((T1(1) <=> T2(0.9)) == std::partial_ordering::greater, "");
        static_assert((T1(1) <=> T2(1.1)) == std::partial_ordering::less, "");
        // Comparisons with NaN are non-constexpr in GCC
        assert((T1(1) <=> T2(std::numeric_limits<double>::quiet_NaN()))
                      == std::partial_ordering::unordered);
    }
    {
        typedef std::tuple<long, float> T1;
        typedef std::tuple<double, int> T2;
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::partial_ordering);
        static_assert((T1(1, 2) <=> T2(1, 2)) == std::partial_ordering::equivalent, "");
        static_assert((T1(1, 2) <=> T2(0.9, 2)) == std::partial_ordering::greater, "");
        static_assert((T1(1, 2) <=> T2(1.1, 2)) == std::partial_ordering::less, "");
        static_assert((T1(1, 2) <=> T2(1, 1)) == std::partial_ordering::greater, "");
        static_assert((T1(1, 2) <=> T2(1, 3)) == std::partial_ordering::less, "");
        // Comparisons with NaN are non-constexpr in GCC
        assert((T1(1, 2) <=> T2(std::numeric_limits<double>::quiet_NaN(), 2))
                      == std::partial_ordering::unordered);
        assert((T1(1, std::numeric_limits<float>::quiet_NaN()) <=> T2(1, 2))
                      == std::partial_ordering::unordered);
    }
    {
        typedef std::tuple<short, float, double> T1;
        typedef std::tuple<double, long, int> T2;
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::partial_ordering);
        static_assert((T1(1, 2, 3) <=> T2(1, 2, 3)) == std::partial_ordering::equivalent, "");
        static_assert((T1(1, 2, 3) <=> T2(0.9, 2, 3)) == std::partial_ordering::greater, "");
        static_assert((T1(1, 2, 3) <=> T2(1.1, 2, 3)) == std::partial_ordering::less, "");
        static_assert((T1(1, 2, 3) <=> T2(1, 1, 3)) == std::partial_ordering::greater, "");
        static_assert((T1(1, 2, 3) <=> T2(1, 3, 3)) == std::partial_ordering::less, "");
        static_assert((T1(1, 2, 3) <=> T2(1, 2, 2)) == std::partial_ordering::greater, "");
        static_assert((T1(1, 2, 3) <=> T2(1, 2, 4)) == std::partial_ordering::less, "");
        // Comparisons with NaN are non-constexpr in GCC
        assert((T1(1, 2, 3) <=> T2(std::numeric_limits<double>::quiet_NaN(), 2, 3))
                      == std::partial_ordering::unordered);
        assert((T1(1, std::numeric_limits<float>::quiet_NaN(), 3) <=> T2(1, 2, 3))
                      == std::partial_ordering::unordered);
        assert((T1(1, 2, std::numeric_limits<double>::quiet_NaN()) <=> T2(1, 2, 3))
                      == std::partial_ordering::unordered);
    }
    // Ordering classes and synthesized three way comparison
    {
        typedef std::tuple<long, int, int> T1;
        typedef std::tuple<int, long, int> T2;
        // All strongly ordered members yields strong ordering.
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::strong_ordering);
    }
    {
        struct WeakSpaceship {
            constexpr bool operator==(const WeakSpaceship&) const { return true; }
            constexpr std::weak_ordering operator<=>(const WeakSpaceship&) const {
                return std::weak_ordering::equivalent;
            }
        };
        {
            typedef std::tuple<int, int, WeakSpaceship> T1;
            typedef std::tuple<int, long, WeakSpaceship> T2;
            // Strongly ordered members and a weakly ordered member yields weak ordering.
            ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::weak_ordering);
        }
        {
            typedef std::tuple<int, int, WeakSpaceship> T1;
            typedef std::tuple<double, long, WeakSpaceship> T2;
            // Doubles are partially ordered, so one partial, one strong, and one weak ordering
            // yields partial ordering.
            ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::partial_ordering);
        }
    }
    {
        struct NoSpaceship {
            constexpr bool operator==(const NoSpaceship&) const { return true; }
            constexpr bool operator<(const NoSpaceship&) const { return false; }
        };
        typedef std::tuple<int, int, NoSpaceship> T1;
        typedef std::tuple<int, long, NoSpaceship> T2;
        // Strongly ordered members and a weakly ordered member (synthesized) yields weak ordering.
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::weak_ordering);
    }
    {
        struct SpaceshipNoEquals {
            constexpr std::strong_ordering operator<=>(const SpaceshipNoEquals&) const {
                return std::strong_ordering::equal;
            }
            constexpr bool operator<(const SpaceshipNoEquals&) const { return false; }
        };
        typedef std::tuple<int, int, SpaceshipNoEquals> T1;
        typedef std::tuple<int, long, SpaceshipNoEquals> T2;
        // Spaceship operator with no == operator falls back on the < operator and weak ordering.
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::weak_ordering);
    }
    {
        struct CustomSpaceship {
            constexpr CustomResult operator<=>(const CustomSpaceship&) const { return CustomResult(); }
        };
        typedef std::tuple<int, int, CustomSpaceship> T1;
        typedef std::tuple<short, long, CustomSpaceship> T2;
        // Custom three way return types cannot be used in synthesized three way comparison,
        // but they can be used for (rewritten) operator< when synthesizing a weak ordering.
        ASSERT_SAME_TYPE(decltype(T1() <=> T2()), std::weak_ordering);
    }

  return 0;
}
