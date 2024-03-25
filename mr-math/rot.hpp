#ifndef __rot_hpp_
#define __rot_hpp_

#include "units.hpp"
#include "vec.hpp"
#include "matr.hpp"

namespace mr {
  template <std::floating_point T>
    struct Rotation;

  // aliases
  using Rotf = Rotation<float>;
  using Rotd = Rotation<double>;

  template <std::floating_point T = float>
    struct Yaw {
      const mr::Radians<T> value = 0;
      Yaw() noexcept = default;
      Yaw(const mr::Radians<T> v) : value(v) {}
      operator T() const {return value.value;}
      operator mr::Radians<T>() const {return value;}
    };

  template <std::floating_point T = float>
    struct Pitch {
      const mr::Radians<T> value = 0;
      Pitch() noexcept = default;
      Pitch(const mr::Radians<T> v) : value(v) {}
      operator T() const {return value.value;}
      operator mr::Radians<T>() const {return value;}
    };

  template <std::floating_point T = float>
    struct Roll {
      const mr::Radians<T> value = 0;
      Roll() noexcept = default;
      Roll(const mr::Radians<T> v) : value(v) {}
      operator T() const {return value.value;}
      operator mr::Radians<T>() const {return value;}
    };

  // yaw, pitch, roll (in radians)
  template <std::floating_point T>
    struct [[nodiscard]] Rotation {
      public:
        using MatrT = mr::Matr<T, 4>;
        using RowT = typename MatrT::RowT;

        // default constructor
        constexpr Rotation() noexcept = default;

        // 3 vector constructor
        constexpr Rotation(
            const Vec3<T> &direction,
            const Vec3<T> &right,
            const Vec3<T> &up) noexcept
          : _data (
                RowT(direction, 0), // direction
                RowT(right, 0),     // right
                RowT(up, 0),        // up
                RowT(0, 0, 0, 0)
            ) {}

        // copy semantics
        constexpr Rotation(const Rotation &other) noexcept = default;
        constexpr Rotation & operator=(const Rotation &other) noexcept = default;

        // move semantics
        constexpr Rotation(Rotation &&other) noexcept = default;
        constexpr Rotation & operator=(Rotation &&other) noexcept = default;

        // angle in radians
        constexpr Rotation & operator+=(Pitch<T> angle_rad) noexcept {
          _data *= Matr4<T>::rotate(angle_rad, mr::Vec3f(right()));

          return *this;
        }

        // angle in radians
        constexpr Rotation & operator+=(Yaw<T> angle_rad) noexcept {
          _data *= Matr4<T>::rotate(angle_rad, mr::Vec3f(up()));

          return *this;
        }

        // angle in radians
        constexpr Rotation & operator+=(Roll<T> angle_rad) noexcept {
          _data *= Matr4<T>::rotate(angle_rad, mr::Vec3f(direction()));

          return *this;
        }

        // getters
        constexpr Vec3<T> direction() const noexcept {
          return Vec3<T>(_data[0]);
        }

        constexpr Vec3<T> right() const noexcept {
          return Vec3<T>(_data[1]);
        }

        constexpr Vec3<T> up() const noexcept {
          return Vec3<T>(_data[2]);
        }

      private:
        MatrT _data {
          RowT(0, 0, -1, 0), // direction
          RowT(1, 0, 0, 0),  // right
          RowT(0, 1, 0, 0),  // up
          RowT(0, 0, 0, 0)   // dummy
        };
    };
}

#endif // __rot_hpp_
