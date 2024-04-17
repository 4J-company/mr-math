#ifndef __def_hpp_
#define __def_hpp_

#include <experimental/simd>
#include <type_traits>
#include <execution>
#include <algorithm>
#include <concepts>
#include <iostream>
#include <sstream>
#include <numeric>
#include <numbers>
#include <ranges>
#include <mutex>
#include <cmath>
#include <bit>
#ifdef __cpp_lib_format
  #include <format>
#endif

namespace stdx {
  using namespace std::experimental;
  using namespace std::experimental::__proposed;
}

#if defined(__SSE__)
#include <immintrin.h>
#include <xmmintrin.h>
#elif defined(__aarch64__)
#include <arm64_neon.h>
#elif defined(__arm__)
#include <arm_neon.h>
#endif

namespace mr {
  template <typename T>
    concept ArithmeticT = std::integral<T> || std::floating_point<T>;

  template <std::size_t ... Is1>
    inline constexpr auto zero_index_sequence_impl(
        std::index_sequence<Is1...> const &)
    -> decltype( std::index_sequence<(Is1, 0)...>{} );

  template <std::size_t S>
    using zero_index_sequence = decltype(zero_index_sequence_impl(std::make_index_sequence<S>{}));

  template<typename Seq1, typename Seq2>
    struct concat_sequence;

  template<std::size_t... Ints1, std::size_t... Ints2>
    struct concat_sequence<std::index_sequence<Ints1...>, std::index_sequence<Ints2...>> {
      using type = std::index_sequence<Ints1..., Ints2...>;
    };

  template<typename Seq1, typename Seq2>
    using concat_sequence_t = typename concat_sequence<Seq1, Seq2>::type;

  template <std::size_t N, std::size_t... Is>
    requires (sizeof...(Is) < N)
    using resized_index_sequence =
      concat_sequence_t<
        std::index_sequence<Is...>,
        zero_index_sequence<N - sizeof...(Is)>
      >;


  // fast inverse implementation for floats
  // use 1 / std::sqrt for higher precision
  template <ArithmeticT T> requires(sizeof(T) == sizeof(float))
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

  // fast inverse implementation for doubles
  // use 1 / std::sqrt for higher precision
  template <ArithmeticT T> requires(sizeof(T) == sizeof(double))
    constexpr double finv_sqrt(T number) {
      unsigned long long i;
      double x2, y;
      const double threehalfs = 1.5F;

      x2 = number * 0.5F;
      y = number;
      i = std::bit_cast<unsigned long long>(y); // evil floating point bit level hacking
      i = 0x5fe6f7ced9168800 - (i >> 1);      // what the fuck?
      y = std::bit_cast<double>(i);
      y = y * (threehalfs - (x2 * y * y)); // 1st iteration

      return y;
    }

  // fast inverse implementation for floats without Newton iteration
  // use finv_sqrt or for higher precision (1 / std::sqrt for even higher)
  template <ArithmeticT T> requires(sizeof(T) == sizeof(float))
    constexpr float ffinv_sqrt(T number) {
      unsigned i;
      float y;

      y = number;
      i = std::bit_cast<unsigned>(y); // evil floating point bit level hacking
      i = 0x5f3759df - (i >> 1);      // what the fuck?
      y = std::bit_cast<float>(i);

      return y;
    }

  // fast inverse implementation for doubles without Newton iteration
  // use finv_sqrt or for higher precision (1 / std::sqrt for even higher)
  template <ArithmeticT T> requires(sizeof(T) == sizeof(double))
    constexpr double ffinv_sqrt(T number) {
      unsigned long long i;
      double y;

      y = number;
      i = std::bit_cast<unsigned long long>(y); // evil floating point bit level hacking
      i = 0x5fe6f7ced9168800 - (i >> 1);      // what the fuck?
      y = std::bit_cast<double>(i);

      return y;
    }
} // namespace mr

#endif // __def_hpp_
