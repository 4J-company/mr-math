#ifndef __MR_ROW_HPP_
#define __MR_ROW_HPP_

#include "operators.hpp"

namespace mr {
inline namespace math {
  template <ArithmeticT T>
    struct Quat;

  template <ArithmeticT T, std::size_t N>
    struct alignas(T) Row : RowOperators<Row<T, N>> {
      friend struct Quat<T>;

    public:
      using ValueT = T;
      using SimdT = SimdImpl<T, N>;
      static constexpr size_t size = N;

      SimdT _data{};

      constexpr Row() noexcept = default;

      // simd-like constructors
      constexpr Row(const SimdT &rhs) noexcept : _data(rhs) {}

      constexpr Row(const T data) : _data(data) {}

      // from elements constructor
      template <ArithmeticT... Args>
      requires (sizeof...(Args) >= 2) && (sizeof...(Args) <= N)
        constexpr Row(Args... args) {
          _set(args...);
        }

      // copy constructor from different row type
      template <ArithmeticT R, std::size_t S>
        constexpr Row(const Row<R, S> &rhs) noexcept {
          _data = stdx::simd_cast<SimdT>(rhs._data);
        }

      template <ArithmeticT R, std::size_t S, ArithmeticT ... Args>
        requires (sizeof...(Args) + S == N)
        constexpr Row(const Row<R, S> &rhs, Args ... args) noexcept {
          [this, &rhs]<size_t ...Is>(std::index_sequence<Is...>) {
            (_set_ind(Is, rhs[Is]), ...);
          }(std::make_index_sequence<S>());

          [this, &args...]<size_t ...Is>(std::index_sequence<Is...>) {
            (_set_ind(S + Is, args), ...);
          }(std::make_index_sequence<sizeof...(Args)>());
        }

      [[nodiscard]] constexpr T operator[](std::size_t i) const {
        return _data[i];
      }

      [[nodiscard]] constexpr T get(std::size_t i) const {
        return _data[i];
      }

      constexpr bool operator==(const Row &other) const noexcept {
        return stdx::all_of(_data == other._data);
      }

      constexpr bool equal(const Row &other, ValueT eps = epsilon<ValueT>()) const noexcept {
        for (size_t i = 0; i < N; i++) {
          if (!mr::equal(_data[i], other._data[i], eps))
            return false;
        }
        return true;
      }

    protected:
      template <ArithmeticT... Args>
        requires (sizeof...(Args) >= 1) && (sizeof...(Args) <= N)
        constexpr void _set(Args... args) noexcept {
          _data = {static_cast<T>(args)...};
        }

    public:
      constexpr void _set_ind(std::size_t ind, T value) noexcept {
        _data[ind] = value;
      }
    };
} // namespace math
} // namespace mr

#ifdef __cpp_lib_format
// std::format support
namespace std {
  template <mr::ArithmeticT T, size_t N>
    struct formatter<mr::Row<T, N>> {
      template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx) {
          // skip all format specifiers
          auto it = ctx.begin();
          while (*it != '}')
              ++it;
          return it;
        }

      template<typename FmtContext>
        auto format(const mr::Row<T, N> &r, FmtContext& ctx) const {
          ostringstream out;
          out << r;
          return ranges::copy(std::move(out).str(), ctx.out()).out;
        }
    };
} // namespace std
#endif

#endif // __MR_ROW_HPP_
