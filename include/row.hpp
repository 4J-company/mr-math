#ifndef __Row_hpp_
#define __Row_hpp_

#include "def.hpp"

namespace mr {
  template <typename T, std::size_t N> requires std::is_arithmetic_v<T>
    struct Row : stdx::fixed_size_simd<T, N> {
      public:
        constexpr Row(const T *data) {
          stdx::fixed_size_simd<T, N>::copy_from(data, stdx::element_aligned);
        }

        template <
          typename ... Args
        > requires ((sizeof...(Args) == 0) || (sizeof...(Args) == N)) &&
                   (std::is_convertible_v<Args, T> && ...)
        constexpr Row(const Args ...args) {
          std::array<T, N> arr {static_cast<T>(args)...};
          stdx::fixed_size_simd<T, N>::copy_from(arr.data(), stdx::element_aligned);
        }

        Row(const stdx::fixed_size_simd<T, N> & other) noexcept {
          stdx::fixed_size_simd<T, N>::operator=(other);
        }

        Row & operator=(const stdx::fixed_size_simd<T, N> & other) noexcept {
          stdx::fixed_size_simd<T, N>::operator=(other);
        }

        friend std::ostream & operator<<(std::ostream &s, const Row &v) noexcept {
          s << '(';
          for (size_t i = 0; i < N; i++)
            s << v[i]
              << (char)(',' * (i < N - 1))
              << (char)(' ' * (i < N - 1));
          s << ')';
          return s;
        }
    };
}

#endif // __Row_hpp_
