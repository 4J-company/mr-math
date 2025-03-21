#ifndef __MR_ROT_HPP_
#define __MR_ROT_HPP_

#include "units.hpp"
#include "vec.hpp"
#include "matr.hpp"
#include "norm.hpp"

namespace mr {
inline namespace math {
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
        using ValueT = T;
        using MatrT = Matr<T, 4>;
        using RowT = typename MatrT::RowT;
        using NormT = Norm3<T>;
        using VecT = Vec3<T>;

        // default constructor
        constexpr Rotation() noexcept = default;

        // 3 vector constructor
        constexpr Rotation(
            const VecT &direction,
            const VecT &right,
            const VecT &up) noexcept
          : _data (
                RowT(direction._data, 0), // direction
                RowT(right._data, 0),     // right
                RowT(up._data, 0),        // up
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
          _data *= MatrT::rotate({right()}, angle_rad);

          return *this;
        }

        // angle in radians
        constexpr Rotation & operator+=(Yaw<T> angle_rad) noexcept {
          _data *= MatrT::rotate({up()}, angle_rad);

          return *this;
        }

        // angle in radians
        constexpr Rotation & operator+=(Roll<T> angle_rad) noexcept {
          _data *= MatrT::rotate({direction()}, angle_rad);

          return *this;
        }

        // setters
        constexpr void direction(NormT dir) noexcept {
          _data[0] = dir._data;
        }

        // getters
        constexpr NormT direction() const noexcept {
          return {unchecked, VecT{_data[0]}};
        }

        constexpr NormT right() const noexcept {
          return {unchecked, VecT{_data[1]}};
        }

        constexpr NormT up() const noexcept {
          return {unchecked, VecT{_data[2]}};
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
}

#endif // __MR_ROT_HPP_
