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
    struct [[nodiscard]] Radians : UnitOperators<Radians<T>> {
    public:
      using ValueT = T;

      static constexpr auto strname = "rad";
      static constexpr std::size_t size = 1;

      T _data;

      constexpr Radians() noexcept {};
      explicit constexpr Radians(T x) noexcept : _data(x) {};

      template <typename U>
        constexpr Radians(const Radians<U> &other) noexcept : _data(other._data) {}

      // conversation operators
      template <typename U>
        explicit constexpr operator Degrees<U>() const noexcept {
          return Degrees<U>{static_cast<U>(_data) *
            std::numbers::inv_pi_v<U> * static_cast<U>(180.)};
        }
      explicit constexpr operator T() const noexcept { return _data; };

      // comparison operator
      [[nodiscard]] friend constexpr auto operator<=>(Radians lhs, Radians rhs) = default;
    };

  template <std::floating_point T>
    struct [[nodiscard]] Degrees : UnitOperators<Degrees<T>> {
    public:
      using ValueT = T;

      static constexpr auto strname = "deg";
      static constexpr std::size_t size = 1;

      T _data;

      explicit constexpr Degrees(T x) noexcept : _data(x) {};

      template <typename U>
        constexpr Degrees(const Degrees<U> &other) noexcept
          : _data(other._data) {}

      // conversation operators
      template <typename U>
        explicit constexpr operator Radians<U>() const noexcept {
          return Radians<U>{static_cast<U>(_data) / static_cast<U>(180.) * std::numbers::pi_v<U>};
        }

      explicit constexpr operator T() const noexcept { return _data; };

      // comparison operator
      [[nodiscard]] friend constexpr auto operator<=>(Degrees lhs, Degrees rhs) = default;
    };

  // internal aliases used by '_rad' and '_deg' suffixes
  using _RadiansLiteral = Radians<float>;
  using _DegreesLiteral = Degrees<float>;

  inline const auto pi = mr::Radiansf(std::numbers::pi_v<float>);

  namespace axis {
    inline mr::Vec3f x {1, 0, 0};
    inline mr::Vec3f y {0, 1, 0};
    inline mr::Vec3f z {0, 0, -1};
  }

  // literals
  constexpr mr::_RadiansLiteral operator"" _rad(unsigned long long value) {
    return mr::_RadiansLiteral{static_cast<float>(value)};
  }

  constexpr mr::_DegreesLiteral operator"" _deg(unsigned long long value) {
    return mr::_DegreesLiteral{static_cast<float>(value)};
  }

  constexpr mr::_RadiansLiteral operator"" _pi(unsigned long long value) {
    return value * mr::pi;
  }

  constexpr mr::_RadiansLiteral operator"" _rad(long double value) {
    return mr::_RadiansLiteral{static_cast<float>(value)};
  }

  constexpr mr::_DegreesLiteral operator"" _deg(long double value) {
    return mr::_DegreesLiteral{static_cast<float>(value)};
  }

  constexpr mr::_RadiansLiteral operator"" _pi(long double value) {
    return value * mr::pi;
  }
}

#endif // __units_hpp_
