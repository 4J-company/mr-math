#ifndef __def_hpp_
#define __def_hpp_

#include <type_traits>
#include <algorithm>
#include <concepts>
#include <iostream>
#include <sstream>
#include <numeric>
#include <optional>
#include <numbers>
#include <ranges>
#include <atomic>
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
  template <typename T>
    concept ArithmeticT = std::integral<T> || std::floating_point<T>;

  template <ArithmeticT T, std::size_t N>
    using SimdImpl = stdx::fixed_size_simd<T, N>;

  template<ArithmeticT T>
    constexpr T epsilon() {
      return std::numeric_limits<T>::epsilon();
    }

  template <ArithmeticT T, ArithmeticT U>
    constexpr bool equal(T a, U b, T epsilon = 0.0001) {
      return std::abs(a - b) < epsilon;
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
  constexpr float fast_rsqrt(float number) {
    // evil floating point bit level hacking
    // what the fuck?
    return std::bit_cast<float>(0x5f3759df - (std::bit_cast<unsigned>(number) >> 1));
  }

  // fast 1 / sqrt implementation for doubles
  // use 1 / std::sqrt() for higher precision
  constexpr double fast_rsqrt(double number) {
    // evil floating point bit level hacking
    // what the fuck?
    return std::bit_cast<double>(0x5fe6f7ced9168800 - (std::bit_cast<unsigned long long>(number) >> 1));
  }

  inline struct UncheckedTag {} unchecked;
} // namespace mr

#endif // __def_hpp_
