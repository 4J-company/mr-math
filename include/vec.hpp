#ifndef __Vec_hpp_
#define __Vec_hpp_

#include "def.hpp"
#include "row.hpp"

namespace mr
{
  template <ArithmeticT T, std::size_t N>
    struct Vec;

  // common types
  template <ArithmeticT T>
    using Vec2 = Vec<T, 2>;
  template <ArithmeticT T>
    using Vec3 = Vec<T, 3>;
  template <ArithmeticT T>
    using Vec4 = Vec<T, 4>;

  using Vec2i = Vec<int, 2>;
  using Vec3i = Vec<int, 3>;
  using Vec4i = Vec<int, 4>;

  using Vec2u = Vec<uint_t, 2>;
  using Vec3u = Vec<uint_t, 3>;
  using Vec4u = Vec<uint_t, 4>;

  using Vec2f = Vec<float, 2>;
  using Vec3f = Vec<float, 3>;
  using Vec4f = Vec<float, 4>;

  using Vec2d = Vec<double, 2>;
  using Vec3d = Vec<double, 3>;
  using Vec4d = Vec<double, 4>;

  template <ArithmeticT T, std::size_t N>
    struct [[nodiscard]] Vec : public Row<T, N>
    {
      using RowT = Row<T, N>;
      static constexpr T epsilon = std::numeric_limits<T>::epsilon();

      // from elements constructor
      template <ArithmeticT... Args>
        requires(sizeof...(Args) <= N) && (std::is_convertible_v<Args, T> && ...)
      constexpr Vec(Args... args) : RowT{args...} {}

      // from simd constructor
      constexpr Vec(RowT data) : RowT(data) {}

      // move semantics
      constexpr Vec(Vec &&) noexcept = default;
      constexpr Vec & operator=(Vec &&) noexcept = default;

      // copy semantics
      constexpr Vec(const Vec &) noexcept = default;
      constexpr Vec & operator=(const Vec &) noexcept = default;

      [[nodiscard]] constexpr T length2() const noexcept {
        return stdx::reduce(RowT::_data * RowT::_data); // sum by default
      }

      [[nodiscard]] constexpr T length() const noexcept {
        return std::sqrt(length2());
      }

      [[nodiscard]] constexpr T inversed_length() const {
        return finv_sqrt(length2());
      }

      // use normalize_fast for higher precision
      constexpr Vec & normalize() noexcept {
        auto len = length2();
        if (std::abs(len) <= epsilon)
          return *this;
        *this *= finv_sqrt(len);
        return *this;
      };

      // use normalized_fast for lower precision
      constexpr Vec normalized() const noexcept {
        auto len = length2();
        if (std::abs(len) <= epsilon)
          return {};
        return {*this * finv_sqrt(len)};
      };

      // use normalize for higher precision
      constexpr Vec & normalize_fast() noexcept {
        auto len = length2();
        if (std::abs(len) <= epsilon)
          return *this;
        *this *= ffinv_sqrt(len);
        return *this;
      };

      // use normalized for higher precision
      constexpr Vec normalized_fast() const noexcept {
        auto len = length2();
        if (std::abs(len) <= epsilon)
          return {};
        return {*this * ffinv_sqrt(len)};
      };

      // cross product
      constexpr Vec cross(const Vec<T, N> &other) const noexcept {
        static_assert(N == 3, "Vectors must have 3 components");

        std::array<T, 3> arr {
          RowT::_data[1] * other._data[2] - RowT::_data[2] * other._data[1],
          RowT::_data[2] * other._data[0] - RowT::_data[0] * other._data[2],
          RowT::_data[0] * other._data[1] - RowT::_data[1] * other._data[0]};

        stdx::fixed_size_simd<T, N> ans;
        ans.copy_from(arr.data(), stdx::element_aligned);
        return {ans};
      }

      constexpr Vec operator%(const Vec<T, N> &other) const noexcept {
        return cross(other);
      }

      // dot product
      [[nodiscard]] constexpr T dot(const Vec<T, N> other) const noexcept {
        return stdx::reduce(RowT::_data * other._data);
      }

      [[nodiscard]] constexpr T operator&(const Vec<T, N> other) const noexcept {
        return dot(other);
      }
    };
} // namespace mr

#endif // __Vec_hpp_
