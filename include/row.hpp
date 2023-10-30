#ifndef __Row_hpp_
#define __Row_hpp_

#include "def.hpp"

namespace mr {
  template <typename T, std::size_t N> requires std::is_arithmetic_v<T>
    struct Row {
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
          _data.copy_from(arr.data(), stdx::element_aligned);
        }

        constexpr Row(const stdx::fixed_size_simd<T, N> & other) noexcept {
          _data = other;
        }
        constexpr Row & operator=(const stdx::fixed_size_simd<T, N> & other) noexcept {
          _data = other;
          return *this;
        }

        // operators returning Row<T, N> type
        constexpr Row operator+(const Row &other) const noexcept {
          return {_data + other._data};
        }
        constexpr Row operator-(const Row &other) const noexcept {
          return {_data - other._data};
        }
        constexpr Row operator*(const Row &other) const noexcept {
          return {_data * other._data};
        }
        constexpr Row operator/(const Row &other) const noexcept {
          return {_data / other._data};
        }
        constexpr Row operator<<(const Row &other) const noexcept {
          return {_data << other._data};
        }
        constexpr Row operator>>(const Row &other) const noexcept {
          return {_data >> other._data};
        }
        template <typename X> requires std::is_scalar_v<X>
        constexpr Row operator*(const X x) const noexcept {
          return {_data * x};
        }
        template <typename X> requires std::is_scalar_v<X>
        constexpr Row operator/(const X x) const noexcept {
          return {_data / x};
        }
        template <typename X> requires std::is_integral_v<X>
        constexpr Row operator<<(const X x) const noexcept {
          return {_data << x};
        }
        template <typename X> requires std::is_integral_v<X>
        constexpr Row operator>>(const X x) const noexcept {
          return {_data >> x};
        }
        constexpr Row & operator+=(const Row &other) noexcept {
          _data += other._data;
          return *this;
        }
        constexpr Row & operator-=(const Row &other) noexcept {
          _data -= other._data;
          return *this;
        }
        constexpr Row & operator*=(const Row &other) noexcept {
          _data *= other._data;
          return *this;
        }
        constexpr Row & operator/=(const Row &other) noexcept {
          _data /= other._data;
          return *this;
        }
        constexpr Row & operator<<=(const Row &other) noexcept {
          _data <<= other._data;
          return *this;
        }
        constexpr Row & operator>>=(const Row &other) noexcept {
          _data >>= other._data;
          return *this;
        }
        template <typename X> requires std::is_scalar_v<X>
        constexpr Row & operator*=(const X x) noexcept {
          _data *= x;
          return *this;
        }
        template <typename X> requires std::is_scalar_v<X>
        constexpr Row & operator/=(const X x) noexcept {
          _data /= x;
          return *this;
        }
        template <typename X> requires std::is_integral_v<X>
        constexpr Row & operator<<=(const X x) noexcept {
          _data <<= x;
          return *this;
        }
        template <typename X> requires std::is_integral_v<X>
        constexpr Row & operator>>=(const X x) noexcept {
          _data >>= x;
          return *this;
        }
        constexpr bool operator<=>(const Row &other) const noexcept = default;

        friend std::ostream & operator<<(std::ostream &s, const Row &v) noexcept {
          s << '(';
          for (size_t i = 0; i < N; i++)
            s << v[i]
              << (char)(',' * (i < N - 1))
              << (char)(' ' * (i < N - 1));
          s << ')';
          return s;
        }

      protected:
        stdx::fixed_size_simd<T, N> _data;
    };
}

#endif // __Row_hpp_
