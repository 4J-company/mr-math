#ifndef __Vec_hpp_
#define __Vec_hpp_

#include "def.hpp"
#include "row.hpp"

namespace mr
{
  // forward declarations
  template <ArithmeticT T, std::size_t N>
  requires (N >= 2)
    struct Vec;

  template <ArithmeticT T>
    struct Vec2;
  template <ArithmeticT T>
    struct Vec3;
  template <ArithmeticT T>
    struct Vec4;

  // common aliases
  using Vec2i = Vec2<int>;
  using Vec3i = Vec3<int>;
  using Vec4i = Vec4<int>;

  using Vec2u = Vec2<uint_t>;
  using Vec3u = Vec3<uint_t>;
  using Vec4u = Vec4<uint_t>;

  using Vec2f = Vec2<float>;
  using Vec3f = Vec3<float>;
  using Vec4f = Vec4<float>;

  using Vec2d = Vec2<double>;
  using Vec3d = Vec3<double>;
  using Vec4d = Vec4<double>;

  // base vector (use aliases for full functional)
  template <ArithmeticT T, std::size_t N>
  requires (N >= 2)
    struct [[nodiscard]] Vec : public Row<T, N>
    {
    public:
      // from elements constructor
      template <ArithmeticT... Args>
        constexpr Vec(Args... args) : RowT{args...} {}

      // from simd constructor
      explicit constexpr Vec(Row<T, N> r) : Row<T, N>(r) {};

      // move semantics
      constexpr Vec(Vec &&) noexcept = default;
      constexpr Vec & operator=(Vec &&) noexcept = default;

      // copy semantics
      constexpr Vec(const Vec &) noexcept = default;
      constexpr Vec & operator=(const Vec &) noexcept = default;

      // length methods
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
        if (std::abs(len) <= _epsilon) [[unlikely]] return *this;
        *this *= finv_sqrt(len);
        return *this;
      };

      // use normalized_fast for lower precision
      constexpr Vec normalized() const noexcept {
        auto len = length2();
        if (std::abs(len) <= _epsilon) [[unlikely]] return {};
        return Vec{*this * finv_sqrt(len)};
      };

      // use normalize for higher precision
      constexpr Vec & normalize_fast() noexcept {
        auto len = length2();
        if (std::abs(len) <= _epsilon) [[unlikely]] return *this;
        *this *= ffinv_sqrt(len);
        return *this;
      };

      // use normalized for higher precision
      constexpr Vec normalized_fast() const noexcept {
        auto len = length2();
        if (std::abs(len) <= _epsilon) [[unlikely]] return {};
        return Vec{*this * ffinv_sqrt(len)};
      };

      constexpr Vec & normalize_fast_unsafe() {
        auto l = length2();
        *this *= ffinv_sqrt(l);
        return *this;
      };

      constexpr Vec normalized_fast_unsafe() const {
        auto l = length2();
        return Vec{*this * ffinv_sqrt(l)};
      };

      // dot product
      [[nodiscard]] constexpr T dot(const Vec<T, N> other) const noexcept {
        return stdx::reduce(RowT::_data * other._data);
      }

      [[nodiscard]] constexpr T operator&(const Vec<T, N> other) const noexcept {
        return dot(other);
      }

      protected:
        using RowT = Row<T, N>;
        static constexpr T _epsilon = std::numeric_limits<T>::epsilon();
    };

  // 2D vector
  template <ArithmeticT T>
    struct [[nodiscard]] Vec2 : public Vec<T, 2>
    {
    public:
      // default special methods
      constexpr Vec2() = default;
      constexpr Vec2(Vec2 &&) noexcept = default;
      constexpr Vec2(const Vec2 &) noexcept = default;
      constexpr Vec2 & operator=(Vec2 &&) noexcept = default;
      constexpr Vec2 & operator=(const Vec2 &) noexcept = default;
      constexpr ~Vec2() = default;

      // constructors
      constexpr Vec2(T x, T y) : VecT(x, y) {}

      constexpr Vec2(T xy) : VecT(xy, xy) {}

      explicit constexpr Vec2(Vec3<T> v) : VecT(v.x(), v.y()) {}

      // setters
      constexpr void set(T x, T y) noexcept { RowT::_set(x, y); }

      constexpr void set(T xy) noexcept { RowT::_set(xy, xy); }

      constexpr void set(const Vec3<T> &v3) noexcept { RowT::_set(v3.x(), v3.y()); }

      constexpr void x(T x) noexcept { set(x, y()); }

      constexpr void y(T y) noexcept { set(x(), y); }

      // getters
      [[nodiscard]] constexpr T x() const noexcept { return (*this)[0]; }

      [[nodiscard]] constexpr T y() const noexcept { return (*this)[1]; }

    private:
      using VecT = Vec<T, 2>;
      using RowT = VecT::RowT;
    };

  // 3D vector
  template <ArithmeticT T>
    struct [[nodiscard]] Vec3 : public Vec<T, 3>
    {
    public:
      // default special methods
      constexpr Vec3() = default;
      constexpr Vec3(Vec3 &&) noexcept = default;
      constexpr Vec3(const Vec3 &) noexcept = default;
      constexpr Vec3 & operator=(Vec3 &&) noexcept = default;
      constexpr Vec3 & operator=(const Vec3 &) noexcept = default;
      constexpr ~Vec3() = default;

      // contrutors
      constexpr Vec3(T x, T y, T z = 0) : VecT(x, y, z) {}

      explicit constexpr Vec3(T xyz) : VecT(xyz, xyz, xyz) {}

      explicit constexpr Vec3(const Vec2<T> &v2, T z = 0) : VecT(v2.x(), v2.y(), z) {}

      explicit constexpr Vec3(const Vec4<T> &v4) : VecT(v4.x(), v4.y(), v4.z()) {}

      // from simd constructor
      explicit constexpr Vec3(Row<T, 3> r) : VecT(r) {};

      // setters
      constexpr void set(T x, T y, T z = 0) noexcept { RowT::_set(x, y, z); }

      constexpr void set(T xyz) noexcept { RowT::_set(xyz, xyz, xyz); }

      constexpr void set(const Vec2<T> &v2, T z = 0) noexcept { RowT::_set(v2.x(), v2.y(), z); }

      constexpr void set(const Vec4<T> &v4) noexcept { RowT::_set(v4.x(), v4.y(), v4.z()); }

      constexpr void x(T x) noexcept { set(x, y(), z()); }

      constexpr void y(T y) noexcept { set(x(), y, z()); }

      constexpr void z(T z) noexcept { set(x(), y(), z); }

      // getters
      [[nodiscard]] constexpr T x() const noexcept { return (*this)[0]; }

      [[nodiscard]] constexpr T y() const noexcept { return (*this)[1]; }

      [[nodiscard]] constexpr T z() const noexcept { return (*this)[2]; }

      // cross product
      constexpr Vec3 cross(const Vec3 &other) const noexcept {

        std::array<T, 3> arr {
          RowT::_data[1] * other._data[2] - RowT::_data[2] * other._data[1],
          RowT::_data[2] * other._data[0] - RowT::_data[0] * other._data[2],
          RowT::_data[0] * other._data[1] - RowT::_data[1] * other._data[0]};

        stdx::fixed_size_simd<T, 3> ans;
        ans.copy_from(arr.data(), stdx::element_aligned);
        return Vec3{ans};
      }

      constexpr Vec3 operator%(const Vec3 &other) const noexcept {
        return cross(other);
      }

    private:
      using VecT = Vec<T, 3>;
      using RowT = VecT::RowT;
    };

    // 3D vector
  template <ArithmeticT T>
    struct [[nodiscard]] Vec4 : public Vec<T, 4>
    {
    public:
      // default special methods
      constexpr Vec4() = default;
      constexpr Vec4(Vec4 &&) noexcept = default;
      constexpr Vec4(const Vec4 &) noexcept = default;
      constexpr Vec4 & operator=(Vec4 &&) noexcept = default;
      constexpr Vec4 & operator=(const Vec4 &) noexcept = default;
      constexpr ~Vec4() = default;

      // contrutors
      constexpr Vec4(T x, T y, T z = 0, T w = 1) : VecT(x, y, z, w) {}

      explicit constexpr Vec4(T xyzw) : VecT(xyzw, xyzw, xyzw, xyzw) {}

      explicit constexpr Vec4(const Vec3<T> &v3, T w = 1) : VecT(v3.x(), v3.y(), v3.z(), w) {}

      // setters
      constexpr void set(T x, T y, T z = 0, T w = 1) noexcept { RowT::_set(x, y, z, w); }

      constexpr void set(T xyzw) noexcept { RowT::_set(xyzw, xyzw, xyzw, xyzw); }

      constexpr void set(const Vec3<T> &v3, T w = 1) noexcept { RowT::_set(v3.x(), v3.y(), v3.z(), w); }

      constexpr void x(T x) noexcept { set(x, y(), z(), w()); }

      constexpr void y(T y) noexcept { set(x(), y, z(), w()); }

      constexpr void z(T z) noexcept { set(x(), y(), z, w()); }

      constexpr void w(T w) noexcept { set(x(), y(), z(), w); }

      // getters
      [[nodiscard]] constexpr T x() const noexcept { return (*this)[0]; }

      [[nodiscard]] constexpr T y() const noexcept { return (*this)[1]; }

      [[nodiscard]] constexpr T z() const noexcept { return (*this)[2]; }

      [[nodiscard]] constexpr T w() const noexcept { return (*this)[3]; }

    private:
      using VecT = Vec<T, 4>;
      using RowT = VecT::RowT;
    };
} // namespace mr

#endif // __Vec_hpp_
