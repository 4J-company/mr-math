#ifndef __units_hpp_
#define __units_hpp_

#include <def.hpp>

namespace mr
{
  // forward declarations
  template <std::floating_point T>
    struct Radians;
  template <std::floating_point T>
    struct Degrees;

  // common alises
  using Radiansf = Radians<float>;
  using Radiansd = Radians<double>;

  // common alises
  using Degreesf = Degrees<float>;
  using Degreesd = Degrees<double>;

  // aliases for '_rad' and '_deg' suffixes
  using _RadiansLiteral = Radians<long double>;
  using _DegreesLiteral = Degrees<long double>;

  //template <std::floating_point T>
    using std::numbers::pi_v;

  template <std::floating_point T>
    struct [[nodiscard]] Radians
    {
    public:
      T value;

      explicit constexpr Radians(T x) noexcept : value(x) {};

      // conversation operators
      explicit constexpr operator Degrees<T>() const noexcept {
        return value * std::numbers::inv_pi_v<T> * static_cast<T>(180.);
      };
      explicit constexpr operator T() const noexcept { return value; };

      // constructor used with '_rad' literal
      constexpr Radians(const _RadiansLiteral &other) noexcept : value(other.value) {};
            // reference used for compilation ^

      // arithmetic operators
      constexpr Radians operator-() const noexcept {
        return Radians{-value};
      }
      constexpr Radians operator+(Radians other) const noexcept {
        return Radians{value + other.value};
      }
      constexpr Radians operator-(Radians other) const noexcept {
        return Radians{value - other.value};
      }
      constexpr Radians operator*(T other) const noexcept {
        return Radians{value * other};
      }
      constexpr Radians operator/(T other) const noexcept {
        return Radians{value / other};
      }
      friend constexpr Radians operator*(T left, Radians right) noexcept {
        return Radians{right.value * left};
      }
      friend constexpr Radians operator/(T left, Radians right) noexcept {
        return Radians{right.value / left};
      }

      // assign operators
      constexpr Radians & operator+=(Radians other) noexcept {
        value += other.value;
        return *this;
      }
      constexpr Radians & operator-=(Radians other) noexcept {
        value -= other.value;
        return *this;
      }
      constexpr Radians & operator*=(T other) noexcept {
        value *= other;
        return *this;
      }
      constexpr Radians & operator/=(T other) noexcept {
        value /= other;
        return *this;
      }

      // comparison operator
      [[nodiscard]] friend constexpr auto operator<=>(Radians left, Radians right﻿) = default;

      // output operator
      friend std::ostream & operator<<(std::ostream &stream, Radians radians) noexcept {
        stream << radians.value << "rad";
        return stream;
      }
    };

  template <std::floating_point T>
    struct [[nodiscard]] Degrees
    {
    public:
      T value;

      explicit constexpr Degrees(T x) noexcept : value(x) {};

      // conversation operators
      explicit constexpr operator Radians<T>() const noexcept {
        return Radians<T>{value / static_cast<T>(180.) * std::numbers::pi_v<T>};
      };
      explicit constexpr operator T() const noexcept { return value; };

      // constructor used with '_deg' literal
      constexpr Degrees(const _DegreesLiteral &other) noexcept : value(other.value) {};
            // reference used for compilation ^

      // arithmetic operators
      constexpr Degrees operator-() const noexcept {
        return Degrees{-value};
      }
      constexpr Degrees operator+(Degrees other) const noexcept {
        return Degrees{value + other.value};
      }
      constexpr Degrees operator-(Degrees other) const noexcept {
        return Degrees{value - other.value};
      }
      constexpr Degrees operator*(T other) const noexcept {
        return Degrees{value * other};
      }
      constexpr Degrees operator/(T other) const noexcept {
        return Degrees{value / other};
      }
      friend constexpr Degrees operator*(T left, Degrees right) noexcept {
        return Degrees{right.value * left};
      }
      friend constexpr Degrees operator/(T left, Degrees right) noexcept {
        return Degrees{right.value / left};
      }

      // assign operators
      constexpr Degrees & operator+=(Degrees other) noexcept {
        value += other.value;
        return *this;
      }
      constexpr Degrees & operator-=(Degrees other) noexcept {
        value -= other.value;
        return *this;
      }
      constexpr Degrees & operator*=(T other) noexcept {
        value *= other;
        return *this;
      }
      constexpr Degrees & operator/=(T other) noexcept {
        value /= other;
        return *this;
      }

      // comparison operator
      [[nodiscard]] friend constexpr bool operator<=>(Degrees left, Degrees right﻿) = default;

      // output operator
      friend std::ostream & operator<<(std::ostream &stream, Degrees Degrees) noexcept {
        stream << Degrees.value << "deg";
        return stream;
      }
    };
}

  // literals
  constexpr mr::_RadiansLiteral operator"" _rad(long double value) {
    return mr::_RadiansLiteral{value};
  }

  constexpr mr::_DegreesLiteral operator"" _deg(long double value) {
    return mr::_DegreesLiteral{value};
  }

#endif // __units_hpp_
