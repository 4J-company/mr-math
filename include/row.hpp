#ifndef __Row_hpp_
#define __Row_hpp_

#include "def.hpp"

namespace mr
{
  template <ArithmeticT T, std::size_t N>
    struct Row
    {
    public:
      // default constructor
      constexpr Row() noexcept = default;

      // from const constructor
      constexpr Row(const T data) : _data(data) {}

      // from elements pointer constructor
      constexpr Row(const T *data) {
        _data.copy_from(data, stdx::element_aligned);
      }

      // from elements constructor
      template <ArithmeticT... Args> requires (sizeof...(Args) >= 2 && sizeof...(Args) <= N)
        constexpr Row(Args... args) {
          _set(args...);
        }

      // copy from simd semantic
      template <ArithmeticT R, std::size_t S>
        constexpr Row(const Row<R, S> &other) noexcept {
          // size conversion
          std::array<R, std::max(S, N)> tmp1;
          other._data.copy_to(tmp1.data(), stdx::element_aligned);

          stdx::fixed_size_simd<R, N> tmp2;
          tmp2.copy_from(tmp1.data(), stdx::element_aligned);

          // type conversion
          _data = tmp2;
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
      // copy from simd semantic
      template <ArithmeticT R, std::size_t S, ArithmeticT ... Args>
        requires (sizeof...(Args) + S == N)
        constexpr Row(const Row<R, S> &other, Args ... args) noexcept {
          // size conversion
          std::array<R, N> tmp1;
          other._data.copy_to(tmp1.data(), stdx::element_aligned);

          Row tmp3 {static_cast<R>(args)...};
          tmp3._data.copy_to(tmp1.data() + S, stdx::element_aligned);

          stdx::fixed_size_simd<R, N> tmp2;
          tmp2.copy_from(tmp1.data(), stdx::element_aligned);

          // type conversion
          _data = tmp2;
        }
#pragma GCC diagnostic pop

      template <ArithmeticT R>
        constexpr Row(const stdx::fixed_size_simd<R, N> &other) noexcept {
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

      constexpr void _set_ind(std::size_t ind, T value) noexcept {
        std::array<T, N> arr;
        _data.copy_to(arr.data(), stdx::element_aligned);
        arr[ind] = value;
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
