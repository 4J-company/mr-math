#ifndef __units_hpp_
#define __units_hpp_

#include <def.hpp>

namespace mr
{
  // type that has only 'ArithmeticT value' member
  // and supports arithmetics operations
  template <typename T>
    concept UnitT = requires(T unit)
    {
      requires ArithmeticT<T>;

      // constructor check
      T{typename T::ValueT{}};

      // operations check
      {unit + unit} -> std::same_as<T>;
      {unit - unit} -> std::same_as<T>;
      {unit * ArithmeticT<T>} -> std::same_as<T>;
      {unit / ArithmeticT<T>} -> std::same_as<T>;
    };

  // extract value from unit type (used for units compatibility)
  template <typename U>
    struct GetValue { using T = U; };
  template <UnitT U>
    struct GetValue<U> { using T = U::ValueT; };

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


  template <std::floating_point T>
    struct [[nodiscard]] Radians
    {
    public:
      using ValueT = T;

      T value;

      constexpr Radians() noexcept {};
      explicit constexpr Radians(T x) noexcept : value(x) {};

      template <typename U>
        constexpr Radians(const Radians<U> &other) noexcept : value(other.value) {}

      // conversation operators
      template <typename U>
        explicit constexpr operator Degrees<U>() const noexcept {
          return Degrees<U>{static_cast<U>(value) *
            std::numbers::inv_pi_v<U> * static_cast<U>(180.)};
        }
      explicit constexpr operator T() const noexcept { return value; };

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
      [[nodiscard]] friend constexpr auto operator<=>(Radians left, Radians right) = default;

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
      using ValueT = T;

      T value;

      explicit constexpr Degrees(T x) noexcept : value(x) {};

      template <typename U>
        constexpr Degrees(const Degrees<U> &other) noexcept
          : value(other.value) {}

      // conversation operators
      template <typename U>
        explicit constexpr operator Radians<U>() const noexcept {
          return Radians<U>{static_cast<U>(value) / static_cast<U>(180.) * std::numbers::pi_v<U>};
        }
      explicit constexpr operator T() const noexcept { return value; };

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
      [[nodiscard]] friend constexpr auto operator<=>(Degrees left, Degrees right) = default;

      // output operator
      friend std::ostream & operator<<(std::ostream &stream, Degrees Degrees) noexcept {
        stream << Degrees.value << "deg";
        return stream;
      }
    };

  // internal aliases used by '_rad' and '_deg' suffixes
  using _RadiansLiteral = Radians<float>;
  using _DegreesLiteral = Degrees<float>;

  inline constexpr auto pi = mr::Radiansf(std::numbers::pi_v<float>);

  namespace axis {
    inline mr::Vec3f x {1, 0, 0};
    inline mr::Vec3f y {0, 1, 0};
    inline mr::Vec3f z {0, 0, -1};
  }

  // literals
  constexpr mr::_RadiansLiteral operator"" _rad(long long value) {
    return mr::_RadiansLiteral{value};
  }

  constexpr mr::_DegreesLiteral operator"" _deg(long long value) {
    return mr::_DegreesLiteral{value};
  }

  constexpr float operator"" _pi(long long value) {
    return value * std::numbers::pi_v<long double>;
  }

  constexpr mr::_RadiansLiteral operator"" _rad(long double value) {
    return mr::_RadiansLiteral{value};
  }

  constexpr mr::_DegreesLiteral operator"" _deg(long double value) {
    return mr::_DegreesLiteral{value};
  }

  constexpr float operator"" _pi(long double value) {
    return value * mr::pi;
  }
}

#endif // __units_hpp_
