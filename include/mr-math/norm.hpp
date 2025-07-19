#ifndef __MR_NORM_HPP_
#define __MR_NORM_HPP_

#include "def.hpp"
#include "vec.hpp"

namespace mr {
inline namespace math {
  template <std::floating_point T>
    struct Rotation;

  // common aliases
  template <ArithmeticT T>
    using Norm2 = Norm<T, 2>;
  template <ArithmeticT T>
    using Norm3 = Norm<T, 3>;
  template <ArithmeticT T>
    using Norm4 = Norm<T, 4>;

  using Norm2i = Norm2<int>;
  using Norm3i = Norm3<int>;
  using Norm4i = Norm4<int>;

  using Norm2u = Norm2<uint32_t>;
  using Norm3u = Norm3<uint32_t>;
  using Norm4u = Norm4<uint32_t>;

  using Norm2f = Norm2<float>;
  using Norm3f = Norm3<float>;
  using Norm4f = Norm4<float>;

  using Norm2d = Norm2<double>;
  using Norm3d = Norm3<double>;
  using Norm4d = Norm4<double>;

  template <ArithmeticT T, std::size_t N> requires (N >= 2)
    struct [[nodiscard]] Norm {
      using ValueT = T;
      using VecT = Vec<T, N>;
      using RowT = Row<T, N>;

      // from elements constructor
      template <ArithmeticT... Args>
      requires (sizeof...(Args) >= 2) && (sizeof...(Args) <= N)
        constexpr Norm(Args... args) : _vec(args...) {
          _vec.normalize();
        }

      constexpr Norm(UncheckedTag, const VecT &v) noexcept : _vec(v) {
          assert(mr::equal(v.length(), 1.0, 0.1));
        }

      constexpr operator const VecT &() const noexcept { return as_vec(); }

      constexpr const VecT &as_vec() const noexcept { return _vec; }
      constexpr const RowT &as_row() const noexcept { return as_vec().row; }

      // getters  
      [[nodiscard]] constexpr T x() const noexcept requires (N >= 1) { return _vec[0]; }
      [[nodiscard]] constexpr T y() const noexcept requires (N >= 2) { return _vec[1]; }
      [[nodiscard]] constexpr T z() const noexcept requires (N >= 3) { return _vec[2]; }
      [[nodiscard]] constexpr T w() const noexcept requires (N >= 4) { return _vec[3]; }
      [[nodiscard]] constexpr T operator[](std::size_t i) const { return _vec[i]; }

      // structured binding support
      template <size_t I> requires (I < N) constexpr T get() const { return _vec[I]; }

      // cross product
      constexpr VecT cross(const VecT &other) const noexcept requires (N == 3) {
        return RowT(as_row().simd.rotated(-1) * other.row.simd.rotated(1)
          - as_row().simd.rotated(1) * other.row.simd.rotated(-1));
#if 0
        std::array<T, 3> arr {
          _vec[1] * other._vec[2] - _vec[2] * other._vec[1],
          _vec[2] * other._vec[0] - _vec[0] * other._vec[2],
          _vec[0] * other._vec[1] - _vec[1] * other._vec[0]
        };

        SimdImpl<T, 3> ans;
        ans.copy_from(arr.data(), stdx::element_aligned);
        return {ans};
#endif
      }

      constexpr Norm cross(const Norm& other) const noexcept requires (N == 3) {
        return Norm{unchecked, cross(other.as_vec())};
      }

      // dot product
      [[nodiscard]] constexpr T dot(const VecT &other) const noexcept {
        return (as_row() * other.row).sum();
      }

      [[nodiscard]] constexpr VecT operator*(const Norm &other) const noexcept {
        return _vec * other._vec;
      }

      // matrix multiplication
      template<ArithmeticT R>
        constexpr VecT operator*(const Matr<R, N> &other) const noexcept {
          VecT tmp {};
          for (size_t i = 0; i < N; i++) {
            tmp.row += (other.rows[i] * _vec[i]).row;
          }
          return tmp;
        }

      template<ArithmeticT R>
        constexpr VecT operator*(const Matr<R, N + 1> &other) noexcept {
          Vec<T, N + 1> copy = Vec<T, N + 1>(*this);
          Vec<T, N + 1> tmp {};

          for (size_t i = 0; i < N; i++) {
            tmp.row += (other.rows[i] * copy.row[i]).row;
          }
          tmp.row += other.rows[N];
          return {tmp};
        }

        friend std::ostream & operator<<(std::ostream &os, const Norm &n) noexcept {
          os << '(';
          for (size_t i = 0; i < N - 1; i++) {
            os << n[i] << ", ";
          }
          os << n[N - 1] << ')';
          return os;
        }

        constexpr friend Norm operator-(const Norm &n) noexcept {
          return {unchecked, -n._vec};
        }

        constexpr bool operator==(const Norm &other) const noexcept {
          return _vec == other._vec;
        }
        constexpr bool operator==(const VecT &other) const noexcept {
          return _vec == other;
        }

        constexpr bool equal(const Norm &other, ValueT eps = epsilon<ValueT>()) const noexcept {
          return _vec.equal(other._vec, eps);
        }
        constexpr bool equal(const VecT &other, ValueT eps = epsilon<ValueT>()) const noexcept {
          return _vec.equal(other, eps);
        }

      private:
        friend struct Vec<T, N>;
        constexpr Norm(const VecT &v) noexcept : Norm(unchecked, v) {}

        VecT _vec;
    };
} // namespace math
} // namespace mr

#ifdef __cpp_structured_bindings
// specializations for structured binding support
namespace std
{
  template <mr::ArithmeticT T, std::size_t N>
  struct tuple_size<mr::Norm<T, N>>
      : std::integral_constant<size_t, N> {};

  template <mr::ArithmeticT T, std::size_t N, std::size_t I>
  struct tuple_element<I, mr::Norm<T, N>> {
    using type = T;
  };
}
#endif

#endif // __MR_NORM_HPP_
