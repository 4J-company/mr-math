#ifndef __MR_DEF_HPP_
#define __MR_DEF_HPP_

#include <type_traits>
#include <algorithm>
#include <concepts>
#include <iostream>
#include <sstream>
#include <numeric>
#include <optional>
#include <cstdint>
#include <numbers>
#include <ranges>
#include <atomic>
#include <array>
#include <mutex>
#include <cmath>
#include <span>
#include <bit>
#ifdef __cpp_lib_format
  #include <format>
#endif

#include "Vc/Vc"

namespace stdx = Vc;

namespace mr {
inline namespace math {
  template <typename T>
    concept ArithmeticT = std::integral<T> || std::floating_point<T>;

  template <ArithmeticT T, std::size_t N>
    using SimdImpl = stdx::fixed_size_simd<T, N>;

  template<ArithmeticT T>
    constexpr T epsilon() {
      return std::numeric_limits<T>::epsilon();
    }

  template <ArithmeticT T, ArithmeticT U>
    constexpr bool equal(T a, U b, std::common_type_t<T, U> eps = epsilon<std::common_type_t<T, U>>()) {
      return std::abs(a - b) <= eps;
    }

  template <typename T, typename U>
    requires requires (T a, U b) {
      a.equal(b, 0.1);
      typename T::ValueT;
      typename U::ValueT;
      requires std::same_as<typename T::ValueT, typename U::ValueT>;
    }
    constexpr bool equal(const T &a, const U &b, typename T::ValueT eps = epsilon<typename T::ValueT>()) {
      return a.equal(b, eps);
    }

  // fast 1 / sqrt implementation for floats
  // use 1 / std::sqrt() for higher precision
  // NOTE: for further info on this function refer to https://hllmn.net/blog/2023-04-20_rsqrt/
  constexpr float fast_rsqrt(float number) {
    // evil floating point bit level hacking
    // what the fuck?
    return std::bit_cast<float>(0x5f3759df - (std::bit_cast<unsigned>(number) >> 1));
  }

  // fast 1 / sqrt implementation for doubles
  // use 1 / std::sqrt() for higher precision
  // NOTE: for further info on this function refer to https://hllmn.net/blog/2023-04-20_rsqrt/
  constexpr double fast_rsqrt(double number) {
    // evil floating point bit level hacking
    // what the fuck?
    return std::bit_cast<double>(0x5fe6eb50c7b537a9 - (std::bit_cast<unsigned long long>(number) >> 1));
  }

  // fast 1 / sqrt implementation for floats
  // use 1 / std::sqrt() for higher precision
  // NOTE: for further info on this function refer to https://hllmn.net/blog/2023-04-20_rsqrt/
  template <std::integral T>
  constexpr float fast_rsqrt(T number) {
    return fast_rsqrt((float)number);
  }

  inline struct UncheckedTag {} unchecked;

  // inclusive numeric interval [low, high]
  // use operator() to check if value is in range
  template<typename L, typename H>
    class Interval {
    public:
      Interval(const L& low_, const H& high_)
        : low(low_), high(high_) {}

      template<typename T>
        requires std::totally_ordered_with<T, L> && std::totally_ordered_with<T, H>
      bool operator()(const T& value) { return low <= value && value <= high; }

      const L& low;
      const H& high;
    };

  // returns inclusive interval
  // usage: if (mr::within_(1, 10)(x))
  template<typename L, typename H>
    constexpr Interval<L, H> within(const L& low, const H& high) {
      return {low, high};
    }

  // exclusive numeric interval (low, high)
  // use operator() to check if value is in range
  template<typename L, typename H>
    class IntervalEx {
    public:
      IntervalEx(const L& low_, const H& high_)
        : low(low_), high(high_) {}

      template<typename T>
        requires std::totally_ordered_with<T, L> && std::totally_ordered_with<T, H>
      bool operator()(const T& value) { return low < value && value < high; }

      const L& low;
      const H& high;
    };

  // returns exclusive interval
  // usage: if (mr::within_ex(1, 10)(x))
  template<typename L, typename H>
    constexpr IntervalEx<L, H> within_ex(const L& low, const H& high) {
      return {low, high};
    }
} // namespace math
} // namespace mr

#endif // __MR_DEF_HPP_
