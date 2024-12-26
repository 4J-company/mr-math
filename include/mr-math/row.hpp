#ifndef __MR_ROW_HPP_
#define __MR_ROW_HPP_

#include "operators.hpp"

namespace mr {
  template <ArithmeticT T>
    struct Quat;

  template <ArithmeticT T, std::size_t N>
    struct Row : RowOperators<Row<T, N>> {
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

// TODO: implement this usign Vc library
#if 0
      constexpr Row(const T *data) {
        _data.copy_from(data, stdx::element_aligned);
      }
#endif

      // from elements constructor
      template <ArithmeticT... Args>
      requires (sizeof...(Args) >= 2) && (sizeof...(Args) <= N)
        constexpr Row(Args... args) {
          _set(args...);
        }

      // copy constructor from different row type
      template <ArithmeticT R, std::size_t S>
        constexpr Row(const Row<R, S> &rhs) noexcept {
          _data = stdx::simd_cast<decltype(_data)>(rhs._data);

#if 0
          // size conversion
          std::array<R, std::max(S, N)> tmp1;
          rhs._data.copy_to(tmp1.data(), stdx::element_aligned);

          stdx::fixed_size_simd<R, N> tmp2;
          tmp2.copy_from(tmp1.data(), stdx::element_aligned);

          // type conversion
          _data = tmp2;
#endif
        }

// TODO: implement this usign Vc library
#if 0
      template <ArithmeticT R, std::size_t S, ArithmeticT ... Args>
        requires (sizeof...(Args) + S == N)
        constexpr Row(const Row<R, S> &rhs, Args ... args) noexcept {
          // size conversion
          std::array<R, N> tmp1;
          rhs._data.copy_to(tmp1.data(), stdx::element_aligned);

          Row tmp3 {static_cast<R>(args)...};
          tmp3._data.copy_to(tmp1.data() + S, stdx::element_aligned);

          stdx::fixed_size_simd<R, N> tmp2;
          tmp2.copy_from(tmp1.data(), stdx::element_aligned);

          // type conversion
          _data = tmp2;
        }
#endif

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

          // std::array<T, N> arr {static_cast<T>(args)...};
          // _data.copy_from(arr.data(), stdx::element_aligned);
        }

    public:
      constexpr void _set_ind(std::size_t ind, T value) noexcept {
        _data[ind] = value;

        // std::array<T, N> arr;
        // _data.copy_to(arr.data(), stdx::element_aligned);
        // arr[ind] = value;
        // _data.copy_from(arr.data(), stdx::element_aligned);
      }
    };
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
          return ranges::copy(move(out).str(), ctx.out()).out;
        }
    };
} // namespace std
#endif

#endif // __MR_ROW_HPP_
