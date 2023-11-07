#ifndef __def_hpp_
#define __def_hpp_

#include <experimental/simd>
#include <type_traits>
#include <execution>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cassert>
#include <ranges>
#include <mutex>
#include <bit>

namespace stdx = std::experimental;

namespace mr
{
  using uint = unsigned int;

  template <typename T>
    concept ArithmeticT = requires { std::is_arithmetic_v<T>; };

  template <ArithmeticT T>
    requires(sizeof(T) == sizeof(float))
    constexpr float finv_sqrt(T number) {
      unsigned i;
      float x2, y;
      const float threehalfs = 1.5F;

      x2 = number * 0.5F;
      y = number;
      i = std::bit_cast<unsigned>(y); // evil floating point bit level hacking
      i = 0x5f3759df - (i >> 1);      // what the fuck?
      y = std::bit_cast<float>(i);
      y = y * (threehalfs - (x2 * y * y)); // 1st iteration

      return y;
    }

  // fast algorithm supports only 32-bit floats
  template <ArithmeticT T>
  requires(sizeof(T) == sizeof(double))
    constexpr float finv_sqrt(T number) {
      return 1 / std::sqrt(number);
    }

  // use finv_sqrt or for higher precision (1 / std::sqrt for even higher)
  template <ArithmeticT T>
  requires(sizeof(T) == sizeof(float))
    constexpr float ffinv_sqrt(T number) {
      unsigned i;
      float x2, y;

      x2 = number * 0.5F;
      y = number;
      i = std::bit_cast<unsigned>(y); // evil floating point bit level hacking
      i = 0x5f3759df - (i >> 1);      // what the fuck?
      y = std::bit_cast<float>(i);

      return y;
    }

  // fast algorithm supports only 32-bit floats
  template <ArithmeticT T>
  requires(sizeof(T) == sizeof(double))
    constexpr float ffinv_sqrt(T number) {
      return 1 / std::sqrt(number);
    }
} // namespace mr

#endif // __def_hpp_
