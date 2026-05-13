#ifndef __MR_UNITS_HPP_
#define __MR_UNITS_HPP_

#include "vec.hpp"

namespace mr {
inline namespace math {
  // type that has only 'ArithmeticT value' member
  // and supports arithmetics operations
  template <typename T>
    concept UnitT = requires(T unit) {
      // member check
      typename T::ValueT;
      sizeof(T) == sizeof(typename T::ValueT);
      requires ArithmeticT<typename T::ValueT>;

      // constructor check
      T{typename T::ValueT{}};

      // operations check
      {unit + unit} -> std::same_as<T>;
      {unit - unit} -> std::same_as<T>;
      {unit * typename T::ValueT{}} -> std::same_as<T>;
      {unit / typename T::ValueT{}} -> std::same_as<T>;
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

      T _data {};

      constexpr Radians() noexcept {};
      explicit constexpr Radians(T x) noexcept : _data(x) {};

      template <typename U>
        constexpr Radians(const Radians<U> &other) noexcept : _data(other._data) {}

      template <typename U>
        constexpr Radians(const Degrees<U>& deg) noexcept
          : _data(static_cast<T>(deg.value()) * std::numbers::pi_v<T> / static_cast<T>(180.)) {}

      explicit constexpr operator T() const noexcept { return _data; };

      constexpr T value() const noexcept { return _data; }

      // comparison operator
      [[nodiscard]] friend constexpr auto operator<=>(Radians lhs, Radians rhs) = default;

      friend std::ostream & operator<<(std::ostream &os, const Radians &r) noexcept {
        os << r._data << "rad";
        return os;
      }
    };

  template <std::floating_point T>
    struct [[nodiscard]] Degrees : UnitOperators<Degrees<T>> {
    public:
      using ValueT = T;

      T _data {};

      explicit constexpr Degrees(T x) noexcept : _data(x) {};

      template <typename U>
        constexpr Degrees(const Degrees<U> &other) noexcept
          : _data(other._data) {}

      template <typename U>
        constexpr Degrees(const Radians<U>& rad) noexcept
          : _data(static_cast<T>(rad.value()) * static_cast<T>(180.) / std::numbers::pi_v<T>) {
        }
      explicit constexpr operator T() const noexcept { return _data; };

      constexpr T value() const noexcept { return _data; }

      // comparison operator
      [[nodiscard]] friend constexpr auto operator<=>(Degrees lhs, Degrees rhs) = default;

      friend std::ostream & operator<<(std::ostream &os, const Degrees &d) noexcept {
        os << d._data << "deg";
        return os;
      }
    };

    template<typename T>
      Degrees(Radians<T>) -> Degrees<T>;

  template <std::floating_point T = float>
    struct Yaw {
      const mr::Radians<T> value = 0;
      Yaw() noexcept = default;
      Yaw(const mr::Radians<T> &v) : value(v) {}
      operator mr::Radians<T>() const {return value;}
    };

  template<typename T>
    Yaw(const Degrees<T>&) -> Yaw<T>;

  template <std::floating_point T = float>
    struct Pitch {
      const mr::Radians<T> value = 0;
      Pitch() noexcept = default;
      Pitch(const mr::Radians<T> &v) : value(v) {}
      operator mr::Radians<T>() const {return value;}
    };

  template<typename T>
    Pitch(const Degrees<T>&) -> Pitch<T>;

  template <std::floating_point T = float>
    struct Roll {
      const mr::Radians<T> value = 0;
      Roll() noexcept = default;
      Roll(const mr::Radians<T> &v) : value(v) {}
      operator mr::Radians<T>() const {return value;}
    };

    template<typename T>
      Roll(const Degrees<T>&) -> Roll<T>;

  inline const auto pi = mr::Radiansf(std::numbers::pi_v<float>);

  namespace axis {
    inline Vec3f x {1, 0, 0};
    inline Vec3f y {0, 1, 0};
    inline Vec3f z {0, 0, -1};
  }

  namespace literals {

    namespace details {
      // internal aliases used by suffixes
      using RadiansLiteral = Radians<float>;
      using DegreesLiteral = Degrees<float>;
    }

    constexpr details::RadiansLiteral operator""_rad(unsigned long long value) {
      return details::RadiansLiteral{static_cast<float>(value)};
    }

    constexpr details::DegreesLiteral operator""_deg(unsigned long long value) {
      return details::DegreesLiteral{static_cast<float>(value)};
    }

    constexpr details::RadiansLiteral operator""_pi(unsigned long long value) {
      return value * pi;
    }

    constexpr details::RadiansLiteral operator""_rad(long double value) {
      return details::RadiansLiteral{static_cast<float>(value)};
    }

    constexpr details::DegreesLiteral operator""_deg(long double value) {
      return details::DegreesLiteral{static_cast<float>(value)};
    }

    constexpr details::RadiansLiteral operator""_pi(long double value) {
      return value * pi;
    }
  }
}
}

#ifdef __cpp_lib_format
// std::format support
namespace std {
  template <mr::UnitT U>
    struct formatter<U> {
      template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx) {
          // skip all format specifiers
          auto it = ctx.begin();
          while (*it != '}')
              ++it;
          return it;
        }

      template<typename FmtContext>
        auto format(U u, FmtContext& ctx) const {
          ostringstream out;
          out << u;
          return ranges::copy(std::move(out).str(), ctx.out()).out;
        }
    };
} // namespace std
#endif

#endif // __MR_UNITS_HPP_
