#ifndef __MR_OPERATORS_HPP_
#define __MR_OPERATORS_HPP_

#include "def.hpp"

namespace mr {
inline namespace math {
  template <typename DerivedT>
    struct UnitOperators {
      friend constexpr DerivedT
      operator+(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs.as_underlying() + rhs.as_underlying()};
      }

      friend constexpr DerivedT
      operator-(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs.as_underlying() - rhs.as_underlying()};
      }

      friend constexpr DerivedT &
      operator+=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs.as_underlying() += rhs.as_underlying();
        return lhs;
      }

      friend constexpr DerivedT &
      operator-=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs.as_underlying() -= rhs.as_underlying();
        return lhs;
      }

      friend constexpr DerivedT &
      operator+(const DerivedT &lhs, const ArithmeticT auto rhs) noexcept {
        return DerivedT{lhs.as_underlying() + rhs};
      }

      friend constexpr DerivedT
      operator-(const DerivedT &lhs, const ArithmeticT auto rhs) noexcept {
        return DerivedT{lhs.as_underlying() - rhs};
      }

      friend constexpr DerivedT &
      operator+=(DerivedT &lhs, const ArithmeticT auto rhs) noexcept {
        lhs.as_underlying() += rhs;
        return lhs;
      }

      friend constexpr DerivedT &
      operator-=(DerivedT &lhs, const ArithmeticT auto rhs) noexcept {
        lhs.as_underlying() -= rhs;
        return lhs;
      }

      friend constexpr DerivedT
      operator*(const DerivedT &lhs, const ArithmeticT auto rhs) noexcept {
        return DerivedT{lhs.as_underlying() * static_cast<DerivedT::ValueT>(rhs)};
      }

      friend constexpr DerivedT
      operator/(const DerivedT &lhs, const ArithmeticT auto rhs) noexcept {
        return DerivedT{lhs.as_underlying() / static_cast<DerivedT::ValueT>(rhs)};
      }

      friend constexpr DerivedT
      operator*(const ArithmeticT auto lhs, const DerivedT &rhs) noexcept {
        return DerivedT{rhs.as_underlying() * static_cast<DerivedT::ValueT>(lhs)};
      }

      friend constexpr DerivedT &
      operator*=(DerivedT &lhs, const ArithmeticT auto x) noexcept {
        lhs.as_underlying() *= static_cast<DerivedT::ValueT>(x);
        return lhs;
      }

      friend constexpr DerivedT &
      operator/=(DerivedT &lhs, const ArithmeticT auto x) noexcept {
        lhs.as_underlying() /= static_cast<DerivedT::ValueT>(x);
        return lhs;
      }

      friend constexpr DerivedT
      operator-(const DerivedT &rhs) noexcept {
        return DerivedT{-rhs.as_underlying()};
      }

      // bitwise operators

      friend constexpr DerivedT
      operator&(const DerivedT& lhs, const DerivedT& rhs) noexcept {
        return DerivedT{lhs.as_underlying() & rhs.as_underlying()};
      }

      friend constexpr DerivedT
      operator&(const DerivedT& lhs, std::integral auto rhs) noexcept {
        return DerivedT{lhs.as_underlying() & rhs};
      }

      friend constexpr DerivedT &
      operator&=(DerivedT& lhs, const DerivedT& rhs) noexcept
      {
        lhs.as_underlying() &= rhs.as_underlying();
        return lhs;
      }

      friend constexpr DerivedT &
      operator&=(DerivedT& lhs, std::integral auto rhs) noexcept
      {
        lhs.as_underlying() &= rhs;
        return lhs;
      }

      friend constexpr DerivedT
      operator|(const DerivedT& lhs, const DerivedT& rhs) noexcept {
        return DerivedT{lhs.as_underlying() | rhs.as_underlying()};
      }

      friend constexpr DerivedT
      operator|(const DerivedT& lhs, std::integral auto rhs) noexcept {
        return DerivedT{lhs.as_underlying() | rhs};
      }

      friend constexpr DerivedT &
      operator|=(DerivedT& lhs, const DerivedT& rhs) noexcept {
        lhs.as_underlying() |= rhs.as_underlying();
        return lhs;
      }

      friend constexpr DerivedT &
      operator|=(DerivedT& lhs, std::integral auto rhs) noexcept {
        lhs.as_underlying() |= rhs;
        return lhs;
      }

      friend constexpr DerivedT
      operator^(const DerivedT& lhs, const DerivedT& rhs) noexcept {
        return DerivedT{lhs.as_underlying() ^ rhs.as_underlying()};
      }

      friend constexpr DerivedT
      operator^(const DerivedT& lhs, std::integral auto rhs) noexcept {
        return DerivedT{lhs.as_underlying() ^ rhs};
      }

      friend constexpr DerivedT &
      operator^=(DerivedT& lhs, const DerivedT& rhs) noexcept {
        lhs.as_underlying() ^= rhs.as_underlying();
        return lhs;
      }

      friend constexpr DerivedT &
      operator^=(DerivedT& lhs, std::integral auto rhs) noexcept {
        lhs.as_underlying() ^= rhs;
        return lhs;
      }

      friend constexpr DerivedT
      operator~(const DerivedT& rhs) noexcept {
        return DerivedT{~rhs.as_underlying()};
      }
    };

  template <typename DerivedT>
    struct RowOperators : UnitOperators<DerivedT> {
      friend constexpr DerivedT
      operator*(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs.as_underlying() * rhs.as_underlying()};
      }

      friend constexpr DerivedT
      operator/(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs.as_underlying() / rhs.as_underlying()};
      }

      friend constexpr DerivedT
      operator<<(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs.as_underlying() << rhs.as_underlying()};
      }

      friend constexpr DerivedT
      operator>>(const DerivedT &lhs, const DerivedT &rhs) noexcept {
        return DerivedT{lhs.as_underlying() >> rhs.as_underlying()};
      }

      friend constexpr DerivedT &
      operator*=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs.as_underlying() *= rhs.as_underlying();
        return lhs;
      }

      friend constexpr DerivedT &
      operator/=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs.as_underlying() /= rhs.as_underlying();
        return lhs;
      }

      friend constexpr DerivedT &
      operator<<=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs.as_underlying() <<= rhs.as_underlying();
        return lhs;
      }

      friend constexpr DerivedT &
      operator>>=(DerivedT &lhs, const DerivedT &rhs) noexcept {
        lhs.as_underlying() >>= rhs.as_underlying();
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
}

#endif // __MR_OPERATORS_HPP_
