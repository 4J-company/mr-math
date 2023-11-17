#ifndef __rot_hpp_
#define __rot_hpp_

#include "def.hpp"
#include "units.hpp"
#include "row.hpp"

namespace mr
{
  template <std::floating_point T>
    struct Rot;

  // aliases
  using Rotf = Rot<float>;
  using Rotd = Rot<double>;

  // yaw, pitch, roll (in radians)
  template <std::floating_point T>
    struct [[nodiscard]] Rot : public Row<T, 3>
    {
    public:
      using RadiansT = Radians<T>;

    private:
      using RowT = Row<T, 3>;

    public:
      // constructors
      Rot() = default;

      // +/- Yaw   - turn right/left
      // +/- Pitch - lean up/down
      // +/- Roll  - rotate CW/CCW
      constexpr Rot(RadiansT Yaw, RadiansT Pitch, RadiansT Roll) : RowT(Yaw.value, Pitch.value, Roll.value) {}

      constexpr Rot(RowT data) : RowT(data) {}

      // copy semantics
      constexpr Rot(const Rot &) noexcept = default;
      constexpr Rot & operator=(const Rot &) noexcept = default;

      // move semantics
      constexpr Rot(Rot &&) noexcept = default;
      constexpr Rot & operator=(Rot &&) noexcept = default;

      // getters & setters
      // +/- Yaw   - turn right/left
      // +/- Pitch - lean up/down
      // +/- Roll  - rotate CW/CCW
      constexpr void set(RadiansT yaw, RadiansT pitch, RadiansT roll) noexcept {
        RowT::set(yaw.value, pitch.value, roll.value);
      }

      constexpr RadiansT yaw() const noexcept {
        return RadiansT{RowT::_data[0]};
      }

      constexpr void yaw(RadiansT yaw) noexcept {
        set(yaw, pitch(), roll());
      }

      constexpr RadiansT pitch() const noexcept {
        return RadiansT{RowT::_data[1]};
      }

      constexpr void pitch(RadiansT pitch) noexcept {
        set(yaw(), pitch, roll());
      }

      constexpr RadiansT roll() const noexcept {
        return RadiansT{RowT::_data[2]};
      }

      constexpr void roll(RadiansT roll) noexcept {
        set(yaw(), pitch(), roll);
      }
    };
}

#endif // __rot_hpp_
