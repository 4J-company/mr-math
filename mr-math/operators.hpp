#ifndef __Operators_hpp__
#define __Operators_hpp__

#include "def.hpp"

namespace mr {
  template <typename DerivedT>
    struct UnitOperators {
      friend constexpr DerivedT
      operator+(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs._data + rhs._data};
      }

      friend constexpr DerivedT
      operator-(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs._data - rhs._data};
      }

      friend constexpr DerivedT &
      operator+=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs._data += rhs._data;
        return lhs;
      }

      friend constexpr DerivedT &
      operator-=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs._data -= rhs._data;
        return lhs;
      }

      friend constexpr DerivedT
      operator*(const DerivedT &lhs, const ArithmeticT auto x) noexcept {
        return DerivedT{lhs._data * static_cast<DerivedT::ValueT>(x)};
      }

      friend constexpr DerivedT
      operator/(const DerivedT &lhs, const ArithmeticT auto x) noexcept {
        return DerivedT{lhs._data / static_cast<DerivedT::ValueT>(x)};
      }

      friend constexpr DerivedT
      operator*(const ArithmeticT auto x, const DerivedT &lhs) noexcept {
        return DerivedT{lhs._data * static_cast<DerivedT::ValueT>(x)};
      }

      friend constexpr DerivedT &
      operator*=(DerivedT &lhs, const ArithmeticT auto x) noexcept {
        lhs._data *= static_cast<DerivedT::ValueT>(x);
        return lhs;
      }

      friend constexpr DerivedT &
      operator/=(DerivedT &lhs, const ArithmeticT auto x) noexcept {
        lhs._data /= static_cast<DerivedT::ValueT>(x);
        return lhs;
      }

      friend constexpr DerivedT
      operator-(const DerivedT &rhs) noexcept {
        return -rhs._data;
      }
    };

  template <typename DerivedT>
    struct RowOperators : UnitOperators<DerivedT> {
      friend constexpr DerivedT
      operator*(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs._data * rhs._data};
      }

      friend constexpr DerivedT
      operator/(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs._data / rhs._data};
      }

      friend constexpr DerivedT
      operator<<(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs._data << rhs._data};
      }

      friend constexpr DerivedT
      operator>>(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs._data >> rhs._data};
      }

      friend constexpr DerivedT &
      operator*=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs._data *= rhs._data;
        return lhs;
      }

      friend constexpr DerivedT &
      operator/=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs._data /= rhs._data;
        return lhs;
      }

      friend constexpr DerivedT &
      operator<<=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs._data <<= rhs._data;
        return lhs;
      }

      friend constexpr DerivedT &
      operator>>=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs._data >>= rhs._data;
        return lhs;
      }

      friend std::ostream &
      operator<<(std::ostream &s, const DerivedT &v) noexcept {
        s << '(';
        for (size_t i = 0; i < DerivedT::size - 1; i++) {
          s << v[i] << ", ";
        }
        s << v[DerivedT::size - 1] << ')';
        return s;
      }
    };
}

#endif // __Operators_hpp__
