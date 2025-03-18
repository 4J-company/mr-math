#ifndef __MR_QUAT_HPP_
#define __MR_QUAT_HPP_

#include "def.hpp"
#include "mr-math/operators.hpp"
#include "vec.hpp"
#include "matr.hpp"
#include "rot.hpp"

namespace mr {
  template <ArithmeticT T> struct Quat;

  using Quatf = Quat<float>;
  using Quatd = Quat<double>;
  using Quati = Quat<int>;
  using Quatu = Quat<unsigned int>;

  template <ArithmeticT T>
    struct Quat {
    private:
      T _angle {};
      Vec3<T> _vec {};

    public:
      constexpr Quat() noexcept = default;
      constexpr Quat(Radians<T> angle, Vec3<T> axis) noexcept {
        axis.normalize();

        T half_angle = angle.value() / 2;

        _angle = std::cos(half_angle);
        _vec = axis * std::sin(half_angle);
      }
      constexpr Quat(Radians<T> a, T x, T y, T z) noexcept : Quat(a, Vec3<T>{x, y, z}) {}
      constexpr Quat(Vec4<T> v) noexcept : _angle(v.x()), _vec(v.y(), v.z(), v.w()) {}
      constexpr Quat(T a, T x, T y, T z) noexcept : Quat(Vec4<T>{a, x, y, z}) {}
      constexpr Quat(T a, Vec3<T> v) noexcept : Quat(Vec4<T>{a, v.x(), v.y(), v.z()}) {}

      // getters
      [[nodiscard]] constexpr Vec3<T> vec() const noexcept { return _vec; }
      [[nodiscard]] constexpr T x() const noexcept { return _vec.x(); }
      [[nodiscard]] constexpr T y() const noexcept { return _vec.y(); }
      [[nodiscard]] constexpr T z() const noexcept { return _vec.z(); }
      [[nodiscard]] constexpr T w() const noexcept { return _angle; }

      [[nodiscard]] explicit constexpr operator Vec4<T>() const noexcept {
        return Vec4<T>{w(), x(), y(), z()};
      }

      [[nodiscard]] constexpr T length2()          const noexcept { return w() * w() + vec().length2(); }
      [[nodiscard]] constexpr T length()           const noexcept { return std::sqrt(length2()); }
      [[nodiscard]] constexpr T inversed_length()  const noexcept { return fast_rsqrt(length2()); }

      // normalize methods
      constexpr Quat & normalize() noexcept {
        auto len = length2();
        if (len <= mr::Vec3<T>::_epsilon) [[unlikely]] return *this;
        auto invlen = 1 / std::sqrt(len);
        _vec *= invlen;
        _angle *= invlen;
        return *this;
      };

      [[nodiscard]] constexpr std::optional<Quat> normalized() const noexcept {
        auto len = length2();
        if (len <= mr::Vec3<T>::_epsilon) [[unlikely]] return std::nullopt;
        auto invlen = 1 / std::sqrt(len);
        auto res = *this;
        res._vec *= invlen;
        res._angle *= invlen;
        return res;
      };

      [[nodiscard]] friend constexpr Quat
      operator+(const Quat &lhs, const Quat &rhs) noexcept {
        return Quat{lhs.w() + rhs.w(), lhs.vec() + rhs.vec()};
      }

      [[nodiscard]] friend constexpr Quat
      operator-(const Quat &lhs, const Quat &rhs) noexcept {
        return Quat{lhs.w() - rhs.w(), lhs.vec() - rhs.vec()};
      }

      friend constexpr Quat &
      operator+=(Quat &lhs, const Quat &rhs) noexcept {
        lhs = lhs + rhs;
        return lhs;
      }

      friend constexpr Quat &
      operator-=(Quat &lhs, const Quat &rhs) noexcept {
        lhs = lhs - rhs;
        return lhs;
      }

      [[nodiscard]] friend constexpr Quat operator*(const Quat &lhs, const Quat &rhs) noexcept {
        return {
          lhs.w() * rhs.w() - lhs.vec().dot(rhs.vec()),
          lhs.w() * rhs.vec() + rhs.w() * lhs.vec() + lhs.vec() % rhs.vec()
        };
      }
      friend constexpr Quat & operator*=(Quat &lhs, const Quat &rhs) noexcept {
        lhs = lhs * rhs;
        return lhs;
      }

      [[nodiscard]] friend constexpr Quat operator*(const Quat &lhs, T rhs) noexcept {
        return {
          lhs.w() * rhs,
          lhs.vec() * rhs,
        };
      }
      friend constexpr Quat & operator*=(Quat &lhs, T rhs) noexcept {
        lhs = lhs * rhs;
        return lhs;
      }

      [[nodiscard]] friend constexpr Vec3<T> operator*(const Vec3<T>& v, const Quat& q) noexcept {
        const Quat pure(0, v.x(), v.y(), v.z());
        const Quat result = q * pure * q.conjugate();
        return result.vec();
      }
      friend constexpr Vec<T, 3> & operator*=(Vec<T, 3> &lhs, const Quat &rhs) noexcept {
        lhs = lhs * rhs;
        return lhs;
      }

      [[nodiscard]] friend constexpr Quat operator/(const Quat &lhs, T rhs) noexcept {
        return {
          lhs.w() / rhs,
          lhs.vec() / rhs,
        };
      }
      friend constexpr Quat & operator/=(Quat &lhs, T rhs) noexcept {
        lhs = lhs / rhs;
        return lhs;
      }

      [[nodiscard]] constexpr Quat conjugate() const noexcept {
        return Quat(w(), -vec());
      }

      friend std::ostream & operator<<(std::ostream &os, const Quat &q) noexcept {
        os << '(' << q.w() << ", " << q.vec() << ')';
        return os;
      }
    };

  template <ArithmeticT T>
  [[nodiscard]] constexpr Quat<T> slerp(Quat<T> q1, Quat<T> q2, float t) noexcept {
    float angle = std::acos(q1.w() * q2.w() + q1.vec().dot(q2.vec()));
    float denom = std::sin(angle);

    return (q1 * std::sin((1 - t) * angle) + q2 * std::sin(t * angle)) / denom;
  }
}

#endif // __MR_QUAT_HPP_
