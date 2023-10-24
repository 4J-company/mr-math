#ifndef __Row_hpp_
#define __Row_hpp_

#include "def.hpp"

namespace mr {
  template <typename T, std::size_t N>
    struct Row : stdx::fixed_size_simd<T, N> {
      public:
        Row(const T *data) {
          stdx::fixed_size_simd<T, N>::copy_from(data, stdx::element_aligned);
        }

        template <
          typename... Args
        > requires ((sizeof...(Args) == N) || (sizeof...(Args) == N)) &&
                   (std::is_convertible_v<Args, T> && ...)
        Row(const Args ...args) {
          std::array<T, N> arr {static_cast<T>(args)...};
          stdx::fixed_size_simd<T, N>::copy_from(arr.data(), stdx::element_aligned);
        }

        friend std::ostream & operator<<(std::ostream &s, const Row &v) noexcept {
          s << '(';
          for (int i = 0; i < N; i++)
            s << v[i]
              << (char)(',' * (i < N - 1))
              << (char)(' ' * (i < N - 1));
          s << ')';
          return s;
        }
    };
}

#endif // __Row_hpp_
