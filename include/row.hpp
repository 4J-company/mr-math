#ifndef __Row_hpp_
#define __Row_hpp_

#include "def.hpp"

namespace mr
{
  template <ArithmeticT T, std::size_t N>
    struct RowImpl
    {
    public:
      // default constructor
      constexpr RowImpl() noexcept = default;

      // from const constructor
      constexpr RowImpl(const T data) : _data(data) {}

      // from elements pointer constructor
      constexpr RowImpl(const T *data) {
        _data.copy_from(data, stdx::element_aligned);
      }

      // from elements constructor
      template <ArithmeticT... Args> requires (sizeof...(Args) >= 2 && sizeof...(Args) <= N)
        constexpr RowImpl(Args... args) {
          _set(args...);
        }

      // copy from simd semantic
      template <ArithmeticT R, std::size_t S>
        constexpr RowImpl(const RowImpl<R, S> &other) noexcept {
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
        constexpr RowImpl(const RowImpl<R, S> &other, Args ... args) noexcept {
          // size conversion
          std::array<R, N> tmp1;
          other._data.copy_to(tmp1.data(), stdx::element_aligned);

          RowImpl tmp3 {static_cast<R>(args)...};
          tmp3._data.copy_to(tmp1.data() + S, stdx::element_aligned);

          stdx::fixed_size_simd<R, N> tmp2;
          tmp2.copy_from(tmp1.data(), stdx::element_aligned);

          // type conversion
          _data = tmp2;
        }
#pragma GCC diagnostic pop

      template <ArithmeticT R>
        constexpr RowImpl(const stdx::fixed_size_simd<R, N> &other) noexcept {
          _data = other;
        }

      constexpr RowImpl & operator=(const stdx::fixed_size_simd<T, N> &other) noexcept {
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
      stdx::fixed_size_simd<T, N> _data {};
    };

  template <ArithmeticT T, std::size_t N, typename ReturnT = RowImpl<T, N>>
    struct RowOperators : virtual RowImpl<T, N> {
      using RowImplT = RowImpl<T, N>;

      // operators returning RowImpl<T, N> type
      constexpr ReturnT operator+(const RowImpl<T, N> &other) const noexcept {
        return {RowImplT::_data + other._data};
      }

      constexpr ReturnT operator-(const RowImpl<T, N> &other) const noexcept {
        return {RowImplT::_data - other._data};
      }

      constexpr ReturnT operator*(const RowImpl<T, N> &other) const noexcept {
        return {RowImplT::_data * other._data};
      }

      constexpr ReturnT operator/(const RowImpl<T, N> &other) const noexcept {
        return {RowImplT::_data / other._data};
      }

      constexpr ReturnT operator<<(const RowImpl<T, N> &other) const noexcept {
        return {RowImplT::_data << other._data};
      }

      constexpr ReturnT operator>>(const RowImpl<T, N> &other) const noexcept {
        return {RowImplT::_data >> other._data};
      }

      // std::experimental::fixed_size_simd operators
      constexpr ReturnT
      operator+(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {RowImplT::_data + other};
      }

      constexpr ReturnT
      operator-(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {RowImplT::_data - other};
      }

      constexpr ReturnT
      operator*(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {RowImplT::_data * other};
      }

      constexpr ReturnT
      operator/(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {RowImplT::_data / other};
      }

      constexpr ReturnT
      operator<<(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {RowImplT::_data << other};
      }

      constexpr ReturnT
      operator>>(const stdx::fixed_size_simd<T, N> &other) const noexcept {
        return {RowImplT::_data >> other};
      }

      // ArithmeticT operators
      template <ArithmeticT X> constexpr ReturnT operator+(const X x) const noexcept {
        return {RowImplT::_data * x};
      }

      template <ArithmeticT X> constexpr ReturnT operator-(const X x) const noexcept {
        return {RowImplT::_data / x};
      }

      template <ArithmeticT X> constexpr ReturnT operator*(const X x) const noexcept {
        return {RowImplT::_data * x};
      }

      template <ArithmeticT X> constexpr ReturnT operator/(X x) const noexcept {
        return {RowImplT::_data / x};
      }

      template <std::integral X> constexpr ReturnT operator<<(X x) const noexcept {
        return {RowImplT::_data << x};
      }

      template <std::integral X> constexpr ReturnT operator>>(X x) const noexcept {
        return {RowImplT::_data >> x};
      }

      // ReturnT operators
      constexpr ReturnT &operator+=(const RowImpl<T, N> &other) noexcept {
        RowImplT::_data += other._data;
        return *this;
      }

      constexpr ReturnT &operator-=(const RowImpl<T, N> &other) noexcept {
        RowImplT::_data -= other._data;
        return *this;
      }

      constexpr ReturnT &operator*=(const RowImpl<T, N> &other) noexcept {
        RowImplT::_data *= other._data;
        return *this;
      }

      constexpr ReturnT &operator/=(const RowImpl<T, N> &other) noexcept {
        RowImplT::_data /= other._data;
        return *this;
      }

      constexpr ReturnT &operator<<=(const RowImpl<T, N> &other) noexcept {
        RowImplT::_data <<= other._data;
        return *this;
      }

      constexpr ReturnT &operator>>=(const RowImpl<T, N> &other) noexcept {
        RowImplT::_data >>= other._data;
        return *this;
      }

      // std::experimental::fixed_size_simd<T, N> operators
      constexpr ReturnT &operator+=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        RowImplT::_data += other;
        return *this;
      }

      constexpr ReturnT &operator-=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        RowImplT::_data -= other;
        return *this;
      }

      constexpr ReturnT &operator*=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        RowImplT::_data *= other;
        return *this;
      }

      constexpr ReturnT &operator/=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        RowImplT::_data /= other;
        return *this;
      }

      constexpr ReturnT &
      operator<<=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        RowImplT::_data <<= other;
        return *this;
      }

      constexpr ReturnT &
      operator>>=(const stdx::fixed_size_simd<T, N> &other) noexcept {
        RowImplT::_data >>= other;
        return *this;
      }

      // ArithmeticT operators
      template <ArithmeticT X> constexpr ReturnT &operator+=(const X x) noexcept {
        RowImplT::_data *= x;
        return *this;
      }

      template <ArithmeticT X> constexpr ReturnT &operator-=(const X x) noexcept {
        RowImplT::_data /= x;
        return *this;
      }

      template <ArithmeticT X> constexpr ReturnT &operator*=(const X x) noexcept {
        RowImplT::_data *= x;
        return *this;
      }

      template <ArithmeticT X> constexpr ReturnT &operator/=(X x) noexcept {
        RowImplT::_data /= x;
        return *this;
      }

      template <std::integral X> constexpr ReturnT &operator<<=(X x) noexcept {
        RowImplT::_data <<= x;
        return *this;
      }

      template <std::integral X> constexpr ReturnT &operator>>=(X x) noexcept {
        RowImplT::_data >>= x;
        return *this;
      }

// #warning "Threeway comparison operator causes ICE"
      [[nodiscard]] bool operator<=>(const RowOperators<T, N, ReturnT> &other) const noexcept = default;

      friend std::ostream &operator<<(std::ostream &s, const ReturnT &v) noexcept {
        s << '(';
        for (size_t i = 0; i < N; i++)
          s << v[i] << (char)(',' * (i < N - 1)) << (char)(' ' * (i < N - 1));
        s << ')';
        return s;
      }
  };

  template <ArithmeticT T, std::size_t N, typename ReturnT = RowImpl<T, N>>
    struct Row : virtual RowImpl<T, N>, RowOperators<T, N, ReturnT> {
      using RowImplT = RowImpl<T, N>;
      using RowOperatorsT = RowOperators<T, N, ReturnT>;
    };
} // namespace mr

#endif // __Row_hpp_
