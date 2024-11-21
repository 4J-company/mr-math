#ifndef __operators_vec_hpp__
#define __operators_vec_hpp__

#include "units.hpp"

namespace mr {
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

#endif // __operators_vec_hpp__
