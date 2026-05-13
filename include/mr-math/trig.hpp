#ifndef __MR_TRIG_HPP_
#define __MR_TRIG_HPP_

#include <concepts>
#include <type_traits>

namespace mr {
  inline namespace math {
    template<typename T>
      concept NumericFloatingPointT =
      std::floating_point<T> ||
      (requires { typename T::ValueT; } && std::floating_point<typename T::ValueT>);

    template <NumericFloatingPointT T>
      T sin(T arg) {
        constexpr T reciprocal_6 = 1.0 / 6.0;
        constexpr T reciprocal_120 = 1.0 / 120.0;
        T arg2 = (arg * arg);
        return arg * (1.0 - arg2 * (reciprocal_6 - arg2 * reciprocal_120));
      }

    template <NumericFloatingPointT T>
      T cos(T arg) {
        constexpr T reciprocal_2  = 1.0 / 2.0;
        constexpr T reciprocal_24 = 1.0 / 24.0;
        T arg2 = arg * arg;
        return 1 - arg2 * (reciprocal_2 - arg2 * reciprocal_24);
      }

    template <NumericFloatingPointT T>
      T tan(T arg) {
        constexpr T c1 = 1.0 / 3.0;
        constexpr T c2 = 2.0 / 15.0;
        T x2 = arg * arg;
        return arg * (1 + x2 * (c1 + x2 * c2));
      }

    template <NumericFloatingPointT T>
      T cot(T arg) {
        constexpr T c1 = 1.0 / 3.0;
        constexpr T c2 = 1.0 / 45.0;
        T x2 = arg * arg;
        return 1 / arg - arg * (c1 + x2 * c2);
      }
  }
}

#endif // __MR_TRIG_HPP_
