#ifndef __Row_hpp_
#define __Row_hpp_

#include "def.hpp"

namespace mr
{
  template <ArithmeticT T, std::size_t N>
    class Matr;

  template <ArithmeticT T, std::size_t N>
    struct Row
    {
    public:
      // default constructor
      constexpr Row() noexcept = default;

      // from elements pointer constructor
      constexpr Row(const T *data) {
        _data.copy_from(data, stdx::element_aligned);
      }

      // from elements constructor
      template <ArithmeticT... Args>
        requires (sizeof...(Args) >= 1)
        constexpr Row(Args... args) {
          _set(args...); // requires sizeof..(Args) <= N
        }

      // copy from simd semantic
      constexpr Row(const stdx::fixed_size_simd<T, N> &other) noexcept {
        _data = other;
      }

      constexpr Row & operator=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        _data = other;
        return *this;
      }

      [[nodiscard]] constexpr T operator[](std::size_t i) const {
        return _data[i];
      }

    protected:
      template <ArithmeticT... Args>
        requires (sizeof...(Args) >= 1) && (sizeof...(Args) <= N) && (std::is_convertible_v<Args, T> && ...)
        constexpr void _set(Args... args) noexcept {
          std::array<T, N> arr {static_cast<T>(args)...};
          _data.copy_from(arr.data(), stdx::element_aligned);
        }

    public:
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

      // std::experimental::fixed_size_simd operators
      constexpr Row operator+(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {_data + other};
      }

      constexpr Row operator-(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {_data - other};
      }

      constexpr Row operator*(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {_data * other};
      }

      constexpr Row operator/(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {_data / other};
      }

      constexpr Row operator<<(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {_data << other};
      }

      constexpr Row operator>>(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {_data >> other};
      }

      // ArithmeticT operators
      template <ArithmeticT X>
        constexpr Row operator+(const X x) const noexcept {
          return {_data * x};
        }

      template <ArithmeticT X>
        constexpr Row operator-(const X x) const noexcept {
          return {_data / x};
        }

      template <ArithmeticT X>
        constexpr Row operator*(const X x) const noexcept {
          return {_data * x};
        }

      template <ArithmeticT X>
        constexpr Row operator/(X x) const noexcept {
          return {_data / x};
        }

      template <std::integral X>
        constexpr Row operator<<(X x) const noexcept {
          return {_data << x};
        }

      template <std::integral X>
        constexpr Row operator>>(X x) const noexcept {
          return {_data >> x};
        }

      // Row operators
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

      // std::experimental::fixed_size_simd<T, N> operators
      constexpr Row & operator+=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        _data += other;
        return *this;
      }

      constexpr Row & operator-=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        _data -= other;
        return *this;
      }

      constexpr Row & operator*=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        _data *= other;
        return *this;
      }

      constexpr Row & operator/=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        _data /= other;
        return *this;
      }

      constexpr Row & operator<<=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        _data <<= other;
        return *this;
      }

      constexpr Row & operator>>=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        _data >>= other;
        return *this;
      }

      // ArithmeticT operators
      template <ArithmeticT X>
        constexpr Row & operator+=(const X x) noexcept {
          _data *= x;
          return *this;
        }

      template <ArithmeticT X>
        constexpr Row & operator-=(const X x) noexcept {
          _data /= x;
          return *this;
        }

      template <ArithmeticT X>
        constexpr Row & operator*=(const X x) noexcept {
          _data *= x;
          return *this;
        }

      template <ArithmeticT X>
        constexpr Row & operator/=(X x) noexcept {
          _data /= x;
          return *this;
        }

      template <std::integral X>
        constexpr Row & operator<<=(X x) noexcept {
          _data <<= x;
          return *this;
        }

      template <std::integral X>
        constexpr Row & operator>>=(X x) noexcept {
          _data >>= x;
          return *this;
        }

      [[nodiscard]] constexpr bool operator<=>(const Row &other) const noexcept = default;

      friend std::ostream & operator<<(std::ostream &s, const Row &v) noexcept {
        s << '(';
        for (size_t i = 0; i < N; i++)
          s << v[i] << (char)(',' * (i < N - 1)) << (char)(' ' * (i < N - 1));
        s << ')';
        return s;
      }

      stdx::fixed_size_simd<T, N> _data {};
    };
} // namespace mr

#endif // __Row_hpp_
