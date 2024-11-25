#ifndef __quat_hpp_
#define __quat_hpp_

#include "def.hpp"
#include "mr-math/operators.hpp"
#include "vec.hpp"
#include "matr.hpp"
#include "rot.hpp"

namespace mr {
  template <ArithmeticT T>
    struct Quat {
    private:
      Radians<T> _angle {};
      Vec3<T> _vec {};

    public:
      constexpr Quat() noexcept = default;
      constexpr Quat(Vec4<T> v) noexcept : _angle(v.x()), _vec(v.y(), v.z(), v.w()) {}
      constexpr Quat(Radians<T> a, Vec3<T> v) noexcept : _angle(a), _vec(v) {}
      constexpr Quat(Radians<T> a, T x, T y, T z) noexcept : _angle(a), _vec(x, y, z) {}

      // getters
      [[nodiscard]] constexpr Vec3<T> vec() const noexcept { return _vec; }
      [[nodiscard]] constexpr T x() const noexcept { return _vec.x(); }
      [[nodiscard]] constexpr T y() const noexcept { return _vec.y(); }
      [[nodiscard]] constexpr T z() const noexcept { return _vec.z(); }
      [[nodiscard]] constexpr T w() const noexcept { return _angle; }

      explicit constexpr operator Vec4<T>() const noexcept {
        return {_angle, _vec.x(), _vec.y(), _vec.z()};
      }

      friend constexpr Quat
      operator+(const Quat &lhs, const Quat &rhs) noexcept {
        return Quat{lhs._angle + rhs._angle, lhs._vec + rhs._vec};
      }

      friend constexpr Quat
      operator-(const Quat &lhs, const Quat &rhs) noexcept {
        return Quat{lhs._angle - rhs._angle, lhs._vec - rhs._vec};
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

      friend constexpr Quat operator*(const Quat &lhs, const Quat &rhs) noexcept {
        return {
          lhs[0] * rhs[0] - lhs & rhs,
          lhs._angle * rhs + rhs._angle * lhs + lhs % rhs
        };
      }
      friend constexpr Quat & operator*=(Quat &lhs, const Quat &rhs) noexcept {
        lhs = lhs * rhs;
        return lhs;
      }

        friend constexpr Vec<T, 3> operator*(const Vec<T, 3> &lhs, const Quat &rhs) noexcept {
          auto vq = rhs._vec * std::cos(rhs._angle._data / 2);
          auto t = vq % lhs;
          auto u = std::sin(rhs._angle._data / 2) * t + vq % t;

          return {lhs + u + u};
        }
      template <std::size_t N>
        friend constexpr Vec<T, N> & operator*=(Vec<T, N> &lhs, const Quat &rhs) noexcept {
          lhs = lhs * rhs;
          return lhs;
        }
    };
}

#endif // __quat_hpp_
