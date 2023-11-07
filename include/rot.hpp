#ifndef __rot_hpp_
#define __rot_hpp_

#include "def.hpp"
#include "row.hpp"

namespace mr
{
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
      constexpr T Yaw() const noexcept {
        return RowT::_data[yaw_index];
      }

      constexpr void Yaw(T yaw) const noexcept {
        RowT::_data[yaw_index] = yaw; // clamp?
      }

      constexpr T Pitch() const noexcept {
        return RowT::_data[pitch_index];
      }

      constexpr void Pitch(T pitch) const noexcept {
        RowT::_data[pitch_index] = pitch; // clamp?
      }

      constexpr T Roll() const noexcept {
        return RowT::_data[roll_index];
      }

      constexpr void Roll(T roll) const noexcept {
        RowT::_data[roll_index] = roll; // clamp?
      }

    private:
      enum
      {
        yaw_index = 0,
        pitch_index,
        roll_index
      };
    };
}

#endif // __rot_hpp_
