#ifndef __MR_QUAT_HPP_
#define __MR_QUAT_HPP_

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
      [[nodiscard]] constexpr T w() const noexcept { return _angle._data; }

      explicit constexpr operator Vec4<T>() const noexcept {
        return Vec4<T>{_angle._data, _vec.x(), _vec.y(), _vec.z()};
      }

      // normalize methods
      constexpr Quat & normalize() noexcept {
        auto len = _vec.length2() + w() * w();
        if (len <= mr::Vec3<T>::_epsilon) [[unlikely]] return *this;
        _vec /= std::sqrt(len);
        _angle /= std::sqrt(len);
        return *this;
      };

      constexpr std::optional<Quat> normalized() const noexcept {
        auto len = _vec.length2() + w() * w();
        if (len <= mr::Vec3<T>::_epsilon) [[unlikely]] return std::nullopt;
        auto res = *this;
        res._vec /= sqrt(len);
        res._angle /= sqrt(len);
        return res;
      };

      friend constexpr Quat
      operator+(const Quat &lhs, const Quat &rhs) noexcept {
        return Quat{mr::Radiansf(lhs.w() + rhs.w()), lhs.vec() + rhs.vec()};
      }

      friend constexpr Quat
      operator-(const Quat &lhs, const Quat &rhs) noexcept {
        return Quat{mr::Radiansf(lhs.w() - rhs.w()), lhs.vec() - rhs.vec()};
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
          mr::Radiansf(lhs.w() * rhs.w() - lhs.vec().dot(rhs.vec())),
          lhs.w() * rhs.vec() + rhs.w() * lhs.vec() + lhs.vec() % rhs.vec()
        };
      }
      friend constexpr Quat & operator*=(Quat &lhs, const Quat &rhs) noexcept {
        lhs = lhs * rhs;
        return lhs;
      }

        friend constexpr Vec<T, 3> operator*(const Vec<T, 3> &lhs, const Quat &rhs) noexcept {
          auto vq = rhs.vec() * std::cos(rhs.w() / 2);
          auto t = vq % lhs;
          auto u = std::sin(rhs.w() / 2) * t + vq % t;

<<<<<<< Updated upstream
          return {lhs + u + u};
        }
      template <std::size_t N>
        friend constexpr Vec<T, N> & operator*=(Vec<T, N> &lhs, const Quat &rhs) noexcept {
          lhs = lhs * rhs;
          return lhs;
        }
=======
      [[nodiscard]] friend constexpr Vec3<T> operator*(const Vec3<T>& v, const Quat& q) noexcept {
        const Quat pure(0, v.x(), v.y(), v.z());
        const Quat result = q * pure * q.conjugate();
        std::cout << q << " * " << pure << " * " << q.conjugate() << " = " << result << std::endl;
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
>>>>>>> Stashed changes
    };
}

#endif // __MR_QUAT_HPP_
