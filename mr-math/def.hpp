#ifndef __def_hpp_
#define __def_hpp_

#include <type_traits>
#include <execution>
#include <algorithm>
#include <concepts>
#include <iostream>
#include <sstream>
#include <numeric>
#include <optional>
#include <numbers>
#include <ranges>
#include <mutex>
#include <cmath>
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

  // fast 1 / sqrt implementation for floats
  // use 1 / std::sqrt() for higher precision
  constexpr float fast_rsqrt(float number) {
    unsigned i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = std::bit_cast<unsigned>(y); // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1);      // what the fuck?
    y = std::bit_cast<float>(i);

    return y;
  }

  // fast 1 / sqrt implementation for doubles
  // use 1 / std::sqrt() for higher precision
  constexpr double fast_rsqrt(double number) {
    unsigned long long i;
    double x2, y;
    const double threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = std::bit_cast<unsigned long long>(y); // evil floating point bit level hacking
    i = 0x5fe6f7ced9168800 - (i >> 1);      // what the fuck?
    y = std::bit_cast<double>(i);

    return y;
  }
} // namespace mr

#endif // __def_hpp_
