#ifndef __Row_hpp_
#define __Row_hpp_

#include "def.hpp"

namespace mr {
  template <typename T, std::size_t N>
    using SimdImpl = stdx::fixed_size_simd<T, N>;

  template <ArithmeticT T, std::size_t N, typename ReturnT, typename InputT=void, typename... RestT>
    struct RowOperators : RowOperators<T, N, ReturnT, RestT...> {
      // vectorized data operators
      friend constexpr ReturnT operator+(const ReturnT &self, const InputT &other) noexcept {
        return {self._data + other._data};
      }

      friend constexpr ReturnT operator-(const ReturnT &self, const InputT &other) noexcept {
        return {self._data - other._data};
      }

      friend constexpr ReturnT operator*(const ReturnT &self, const InputT &other) noexcept {
        return {self._data * other._data};
      }

      friend constexpr ReturnT operator/(const ReturnT &self, const InputT &other) noexcept {
        return {self._data / other._data};
      }

      friend constexpr ReturnT operator<<(const ReturnT &self, const InputT &other) noexcept {
        return {self._data << other._data};
      }

      friend constexpr ReturnT operator>>(const ReturnT &self, const InputT &other) noexcept {
        return {self._data >> other._data};
      }

      // ReturnT operators
      friend constexpr ReturnT &operator+=(ReturnT &self, const InputT &other) noexcept {
        self._data += other._data;
        return self;
      }

      friend constexpr ReturnT &operator-=(ReturnT &self, const InputT &other) noexcept {
        self._data -= other._data;
        return self;
      }

      friend constexpr ReturnT &operator*=(ReturnT &self, const InputT &other) noexcept {
        self._data *= other._data;
        return self;
      }

      friend constexpr ReturnT &operator/=(ReturnT &self, const InputT &other) noexcept {
        self._data /= other._data;
        return self;
      }

      friend constexpr ReturnT &operator<<=(ReturnT &self, const InputT &other) noexcept {
        self._data <<= other._data;
        return self;
      }

      friend constexpr ReturnT &operator>>=(ReturnT &self, const InputT &other) noexcept {
        self._data >>= other._data;
        return self;
      }
  };

  template <ArithmeticT T, std::size_t N, typename ReturnT>
    struct RowOperators<T, N, ReturnT, void> {
      using SimdImplT = SimdImpl<T, N>;

      // std::experimental::fixed_size_simd operators
      friend constexpr ReturnT
      operator+(const ReturnT &self, const SimdImplT &other) noexcept {
        return {self._data + other};
      }

      friend constexpr ReturnT
      operator-(const ReturnT &self, const SimdImplT &other) noexcept {
        return {self._data - other};
      }

      friend constexpr ReturnT
      operator*(const ReturnT &self, const SimdImplT &other) noexcept {
        return {self._data * other};
      }

      friend constexpr ReturnT
      operator/(const ReturnT &self, const SimdImplT &other) noexcept {
        return {self._data / other};
      }

      friend constexpr ReturnT
      operator<<(const ReturnT &self, const SimdImplT &other) noexcept {
        return {self._data << other};
      }

      friend constexpr ReturnT
      operator>>(const ReturnT &self, const SimdImplT &other) noexcept {
        return {self._data >> other};
      }

      friend constexpr ReturnT &operator+=(ReturnT &self, const SimdImplT &other) noexcept {
        self._data += other;
        return self;
      }

      friend constexpr ReturnT &operator-=(ReturnT &self, const SimdImplT &other) noexcept {
        self._data -= other;
        return self;
      }

      friend constexpr ReturnT &operator*=(ReturnT &self, const SimdImplT &other) noexcept {
        self._data *= other;
        return self;
      }

      friend constexpr ReturnT &operator/=(ReturnT &self, const SimdImplT &other) noexcept {
        self._data /= other;
        return self;
      }

      friend constexpr ReturnT &
      operator<<=(ReturnT &self, const SimdImplT &other) noexcept {
        self._data <<= other;
        return self;
      }

      friend constexpr ReturnT &
      operator>>=(ReturnT &self, const SimdImplT &other) noexcept {
        self._data >>= other;
        return self;
      }

      friend constexpr ReturnT
      operator+(const SimdImplT &other, const ReturnT &self) noexcept {
        return {self._data + other};
      }

      friend constexpr ReturnT
      operator-(const SimdImplT &other, const ReturnT &self) noexcept {
        return {self._data - other};
      }

      friend constexpr ReturnT
      operator*(const SimdImplT &other, const ReturnT &self) noexcept {
        return {self._data * other};
      }

      friend constexpr ReturnT
      operator/(const SimdImplT &other, const ReturnT &self) noexcept {
        return {self._data / other};
      }

      friend constexpr ReturnT
      operator<<(const SimdImplT &other, const ReturnT &self) noexcept {
        return {self._data << other};
      }

      friend constexpr ReturnT
      operator>>(const SimdImplT &other, const ReturnT &self) noexcept {
        return {self._data >> other};
      }

      friend constexpr ReturnT &operator+=(SimdImplT &other, const ReturnT &self) noexcept {
        other += self._data;
        return other;
      }

      friend constexpr ReturnT &operator-=(SimdImplT &other, const ReturnT &self) noexcept {
        other -= self._data;
        return other;
      }

      friend constexpr ReturnT &operator*=(SimdImplT &other, const ReturnT &self) noexcept {
        other *= self._data;
        return other;
      }

      friend constexpr ReturnT &operator/=(SimdImplT &other, const ReturnT &self) noexcept {
        other /= self._data;
        return other;
      }

      friend constexpr ReturnT &
      operator<<=(SimdImplT &other, const ReturnT &self) noexcept {
        other <<= self._data;
        return other;
      }

      friend constexpr ReturnT &
      operator>>=(SimdImplT &other, const ReturnT &self) noexcept {
        other >>= self._data;
        return other;
      }

      // ArithmeticT operators
      template <ArithmeticT X> friend constexpr ReturnT operator+(const ReturnT &self, const X x) noexcept {
        return {self._data + x};
      }

      template <ArithmeticT X> friend constexpr ReturnT operator-(const ReturnT &self, const X x) noexcept {
        return {self._data - x};
      }

      template <ArithmeticT X> friend constexpr ReturnT operator*(const ReturnT &self, const X x) noexcept {
        return {self._data * x};
      }

      template <ArithmeticT X> friend constexpr ReturnT operator/(const ReturnT &self, X x) noexcept {
        return {self._data / x};
      }

      template <std::integral X> friend constexpr ReturnT operator<<(const ReturnT &self, X x) noexcept {
        return {self._data << x};
      }

      template <std::integral X> friend constexpr ReturnT operator>>(const ReturnT &self, X x) noexcept {
        return {self._data >> x};
      }

      template <ArithmeticT X> friend constexpr ReturnT operator+(const X x, const ReturnT &self) noexcept {
        return {self._data + x};
      }

      template <ArithmeticT X> friend constexpr ReturnT operator-(const X x, const ReturnT &self) noexcept {
        return {self._data - x};
      }

      template <ArithmeticT X> friend constexpr ReturnT operator*(const X x, const ReturnT &self) noexcept {
        return {self._data * x};
      }

      template <ArithmeticT X> friend constexpr ReturnT operator/(const X x, const ReturnT &self) noexcept {
        return {self._data / x};
      }

      template <std::integral X> friend constexpr ReturnT operator<<(const X x, const ReturnT &self) noexcept {
        return {self._data << x};
      }

      template <std::integral X> friend constexpr ReturnT operator>>(const X x, const ReturnT &self) noexcept {
        return {self._data >> x};
      }

      template <ArithmeticT X> friend constexpr ReturnT &operator+=(ReturnT &self, const X x) noexcept {
        self._data += x;
        return self;
      }

      template <ArithmeticT X> friend constexpr ReturnT &operator-=(ReturnT &self, const X x) noexcept {
        self._data -= x;
        return self;
      }

      template <ArithmeticT X> friend constexpr ReturnT &operator*=(ReturnT &self, const X x) noexcept {
        self._data *= x;
        return self;
      }

      template <ArithmeticT X> friend constexpr ReturnT &operator/=(ReturnT &self, X x) noexcept {
        self._data /= x;
        return self;
      }

      template <std::integral X> friend constexpr ReturnT &operator<<=(ReturnT &self, X x) noexcept {
        self._data <<= x;
        return self;
      }

      template <std::integral X> friend constexpr ReturnT &operator>>=(ReturnT &self, X x) noexcept {
        self._data >>= x;
        return self;
      }

      friend std::ostream &operator<<(std::ostream &s, const ReturnT &v) noexcept {
        s << '(';
        for (size_t i = 0; i < SimdImplT::size; i++) {
          s << v[i] << (char)(',' * (i < SimdImplT::size - 1)) << (char)(' ' * (i < SimdImplT::size - 1));
        }
        s << ')';
        return s;
      }
    };

  template <ArithmeticT T, std::size_t N>
    struct RowImpl : RowOperators<T, N, RowImpl<T, N>, RowImpl<T, N>> {
    public:
      using SimdImplT = SimdImpl<T, N>;
      static constexpr std::size_t size = N;

      SimdImplT _data {};

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

      template <std::integral I>
      [[nodiscard]] constexpr T operator[](I i) const {
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
    };

  template <ArithmeticT T, std::size_t N>
    struct Row : RowOperators<T, N, Row<T, N>, Row<T, N>, RowImpl<T, N>> {
      using RowImplT = RowImpl<T, N>;

      RowImplT _data;

      // default constructor
      constexpr Row() noexcept = default;

      // from const constructor
      constexpr Row(const T data) : _data(data) {}

      // from elements pointer constructor
      constexpr Row(const T *data) : _data(data) {}

      // from elements constructor
      template <ArithmeticT... Args>
        requires (sizeof...(Args) >= 2 && sizeof...(Args) <= N)
        constexpr Row(Args... args) : _data(args...) {}

      constexpr Row(const RowImplT &row) : _data(row) {
      }

      // copy from simd semantic
      template <ArithmeticT R, std::size_t S>
        constexpr Row(const Row<R, S> &other) noexcept : _data(other._data) {}

      // copy from simd semantic
      template <ArithmeticT R, std::size_t S, ArithmeticT ... Args>
        requires (sizeof...(Args) + S == N)
        constexpr Row(const Row<R, S> &other, Args ... args) noexcept : _data(other._data, args...) {}

      // copy from simd semantic
      template <ArithmeticT R, std::size_t S>
        constexpr Row(const SimdImpl<R, S> &other) noexcept : _data(other) {}

      // copy from simd semantic
      template <ArithmeticT R, std::size_t S, ArithmeticT ... Args>
        requires (sizeof...(Args) + S == N)
        constexpr Row(const SimdImpl<R, S> &other, Args ... args) noexcept : _data(other, args...) {}

      template <ArithmeticT R>
        constexpr Row(const SimdImpl<R, N> &other) noexcept : _data(other) {}

      constexpr Row & operator=(const SimdImpl<T, N> &other) noexcept {
        _data = other;
        return *this;
      }

      [[nodiscard]] constexpr T operator[](std::size_t i) const noexcept {
        return _data[i];
      }
    };
} // namespace mr

#endif // __Row_hpp_
