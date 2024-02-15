#ifndef __Row_hpp_
#define __Row_hpp_

#include "def.hpp"

namespace mr {
  template <typename T, std::size_t N>
    using SimdImpl = stdx::fixed_size_simd<T, N>;

  template <typename ReturnT, typename InputT = void, typename... RestTs>
    struct RowOperators : RowOperators<ReturnT, RestTs...> {

      friend constexpr ReturnT
      operator+(const ReturnT &lhs, const InputT &rhs) noexcept {
        return {lhs._data + rhs._data};
      }

      friend constexpr ReturnT
      operator-(const ReturnT &lhs, const InputT &rhs) noexcept {
        return {lhs._data - rhs._data};
      }

      friend constexpr ReturnT
      operator*(const ReturnT &lhs, const InputT &rhs) noexcept {
        return {lhs._data * rhs._data};
      }

      friend constexpr ReturnT
      operator/(const ReturnT &lhs, const InputT &rhs) noexcept {
        return {lhs._data / rhs._data};
      }

      friend constexpr ReturnT
      operator<<(const ReturnT &lhs, const InputT &rhs) noexcept {
        return {lhs._data << rhs._data};
      }

      friend constexpr ReturnT
      operator>>(const ReturnT &lhs, const InputT &rhs) noexcept {
        return {lhs._data >> rhs._data};
      }

      friend constexpr ReturnT &
      operator+=(ReturnT &lhs, const InputT &rhs) noexcept {
        lhs._data += rhs._data;
        return lhs;
      }

      friend constexpr ReturnT &
      operator-=(ReturnT &lhs, const InputT &rhs) noexcept {
        lhs._data -= rhs._data;
        return lhs;
      }

      friend constexpr ReturnT &
      operator*=(ReturnT &lhs, const InputT &rhs) noexcept {
        lhs._data *= rhs._data;
        return lhs;
      }

      friend constexpr ReturnT &
      operator/=(ReturnT &lhs, const InputT &rhs) noexcept {
        lhs._data /= rhs._data;
        return lhs;
      }

      friend constexpr ReturnT &
      operator<<=(ReturnT &lhs, const InputT &rhs) noexcept {
        lhs._data <<= rhs._data;
        return lhs;
      }

      friend constexpr ReturnT &
      operator>>=(ReturnT &lhs, const InputT &rhs) noexcept {
        lhs._data >>= rhs._data;
        return lhs;
      }
  };

  // specialization for operators with scalars
  template <typename ReturnT>
    struct RowOperators<ReturnT> {

      friend constexpr ReturnT
      operator*(const ReturnT &lhs, const ArithmeticT auto x) noexcept {
        return {lhs._data * x};
      }

      friend constexpr ReturnT
      operator/(const ReturnT &lhs, const ArithmeticT auto x) noexcept {
        return {lhs._data / x};
      }

      friend constexpr ReturnT
      operator*(const ArithmeticT auto x, const ReturnT &lhs) noexcept {
        return {lhs._data * x};
      }

      friend constexpr ReturnT &
      operator*=(ReturnT &lhs, const ArithmeticT auto x) noexcept {
        lhs._data *= x;
        return lhs;
      }

      friend constexpr ReturnT &
      operator/=(ReturnT &lhs, const ArithmeticT auto x) noexcept {
        lhs._data /= x;
        return lhs;
      }

      friend std::ostream &
      operator<<(std::ostream &s, const ReturnT &v) noexcept {
        s << '(';
        for (size_t i = 0; i < ReturnT::size; i++) {
          s << v[i] << (char)(',' * (i < ReturnT::size - 1)) << (char)(' ' * (i < ReturnT::size - 1));
        }
        s << ')';
        return s;
      }
    };

  template <ArithmeticT T, std::size_t N>
    struct Row : RowOperators<Row<T, N>, Row<T, N>> {

    public:
      using SimdT = SimdImpl<T, N>;

      static constexpr size_t size = N;

      SimdT _data{};

      constexpr Row() noexcept = default;

      // simd-like constructors
      constexpr Row(const SimdT &rhs) noexcept : _data(rhs) {}

      constexpr Row(const T data) : _data(data) {}

      constexpr Row(const T *data) {
        _data.copy_from(data, stdx::element_aligned);
      }

      // from elements constructor
      template <ArithmeticT... Args>
      requires (sizeof...(Args) >= 2) && (sizeof...(Args) <= N)
        constexpr Row(Args... args) {
          _set(args...);
        }

      // copy constructor from different row type
      template <ArithmeticT R, std::size_t S>
        requires std::is_convertible_v<R, T>
        constexpr Row(const Row<R, S> &rhs) noexcept {
          // size conversion
          std::array<R, std::max(S, N)> tmp1;
          rhs._data.copy_to(tmp1.data(), stdx::element_aligned);

          stdx::fixed_size_simd<R, N> tmp2;
          tmp2.copy_from(tmp1.data(), stdx::element_aligned);

          // type conversion
          _data = tmp2;
        }

      template <ArithmeticT R, std::size_t S, ArithmeticT ... Args>
        requires (sizeof...(Args) + S == N)
        constexpr Row(const Row<R, S> &rhs, Args ... args) noexcept {
          // size conversion
          std::array<R, N> tmp1;
          rhs._data.copy_to(tmp1.data(), stdx::element_aligned);

          Row tmp3 {static_cast<R>(args)...};
          tmp3._data.copy_to(tmp1.data() + S, stdx::element_aligned);

          stdx::fixed_size_simd<R, N> tmp2;
          tmp2.copy_from(tmp1.data(), stdx::element_aligned);

          // type conversion
          _data = tmp2;
        }

      [[nodiscard]] constexpr T operator[](std::size_t i) const {
        return _data[i];
      }

    protected:
      template <ArithmeticT... Args>
        requires (sizeof...(Args) >= 1) && (sizeof...(Args) <= N)
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
} // namespace mr

#endif // __Row_hpp_
