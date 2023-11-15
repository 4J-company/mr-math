#ifndef __rot_hpp_
#define __rot_hpp_

#include "def.hpp"
#include "row.hpp"

namespace mr
{
  template <std::floating_point T>
    struct Rot;

  // aliases
  using Rotf = Rot<float>;
  using Rotd = Rot<double>;

  // yaw, pitch, roll
  template <std::floating_point T>
    struct [[nodiscard]] Rot : public Row<T, 3>
    {
      using RowT = Row<T, 3>;

      // constructors
      Rot() = default;

      // +/- Yaw   - turn right/left
      // +/- Pitch - lean up/down
      // +/- Roll  - rotate CW/CCW
      constexpr Rot(T Yaw, T Pitch, T Roll) : RowT(Yaw, Pitch, Roll) {}

      constexpr Rot(RowT data) : RowT(data) {}

      // copy semantics
      constexpr Rot(const Rot &) noexcept = default;
      constexpr Rot & operator=(const Rot &) noexcept = default;

      // move semantics
      constexpr Rot(Rot &&) noexcept = default;
      constexpr Rot & operator=(Rot &&) noexcept = default;

      // getters & setters
      constexpr void set(T yaw, T pitch, T roll) noexcept {
        RowT::set(yaw, pitch, roll);
      }

      constexpr T yaw() const noexcept {
        return RowT::_data[0];
      }

      constexpr void yaw(T yaw) noexcept {
        set(yaw, pitch(), roll());
      }

      constexpr T pitch() const noexcept {
        return RowT::_data[1];
      }

      constexpr void pitch(T pitch) noexcept {
        set(yaw(), pitch, roll());
      }

      constexpr T roll() const noexcept {
        return RowT::_data[2];
      }

      constexpr void roll(T roll) noexcept {
        set(yaw(), pitch(), roll);
      }
    };
}

#endif // __rot_hpp_
