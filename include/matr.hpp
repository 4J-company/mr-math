#ifndef __matr_hpp_
#define __matr_hpp_

#include "def.hpp"
#include <iostream>

namespace mr {
  template <typename T, std::size_t N>
    class Vec;
  template <typename T, std::size_t N>
    class Matr;

  template <typename T>
    using Matr4 = Matr<T, 4>;

  using Matr4f = Matr4<float>;
  using Matr4d = Matr4<double>;
  using Matr4i = Matr4<int>;
  using Matr4u = Matr4<uint>;

  template <typename T, std::size_t N>
    class Matr {
      public:
        constexpr static inline std::size_t size = N;

        class Row : stdx::fixed_size_simd<T, N> {
        public:
          template <
            typename... Args,
            typename std::enable_if_t<sizeof...(Args) == 0 || sizeof...(Args) == N, int> = 0,
            typename std::enable_if_t<(std::is_convertible_v<Args, T> && ...), int> = 0
            >
            Row(const Args ...args) {
              std::array<T, N> arr {static_cast<T>(args)...};
              stdx::fixed_size_simd<T, N>::copy_from(arr.data(), stdx::element_aligned);
            }

          friend std::ostream & operator<<(std::ostream &s, const Row &v) noexcept {
            s << '(';
            for (int i = 0; i < N; i++)
              s << v[i] 
                << (char)(',' * (i < N - 1)) 
                << (char)(' ' * (i < N - 1));
            s << ')';
            return s;
          }
        };

      private:
        std::array<Row, N> _data;

      public:
        Matr() = default;

        template <
          class ... Args,
          typename std::enable_if_t<sizeof...(Args) == N, int> = 0,
          typename std::enable_if_t<(std::is_same_v<Row, Args> && ...), int> = 0
          > 
          Matr(Args... args) {
            _data = std::array<Row, N>({static_cast<Row>(args)...});
          }

        // copy semantics
        Matr(const Matr &other) noexcept = default;
        Matr & operator=(const Matr &other) noexcept = default;

        // move semantics
        Matr(Matr &&other) noexcept = default;
        Matr & operator=(Matr &&other) noexcept = default;

        [[nodiscard]] constexpr Matr operator*=(const Matr &other) noexcept {
          const Matr<T, N> tr = other.transposed();
          for (int i = 0; i < N; i++)
            _data[i] *= tr._data[i];
          return *this;
        }

        [[nodiscard]] constexpr Matr operator+=(const Matr &other) noexcept {
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            _data[i] += other._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr Matr operator-=(const Matr &other) noexcept {
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            _data[i] -= other._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr Matr operator*(const Matr &other) const noexcept {
          const Matr<T, N> tr = other.transposed();
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            tmp[i] = _data[i] + tr._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr Matr operator+(const Matr &other) const noexcept {
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            tmp[i] = _data[i] + other._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr Matr operator-(const Matr &other) const noexcept {
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            tmp[i] = _data[i] - other._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr T operator!() const noexcept {
          return {};
        }

        [[nodiscard]] constexpr Matr transposed() const noexcept {
          std::array<Row, N> tmp1;
          std::array<std::array<T, N>, N> tmp2;
          for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
              tmp2[i][j] = _data[j][i];
          for (int i = 0; i < N; i++)
            tmp1[i].copy_from(tmp2[i], stdx::element_aligned);
          return {tmp1};
        }

        [[nodiscard]] constexpr Matr & transpose() noexcept {
          std::array<std::array<T, N>, N> tmp2;
          for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
              tmp2[i][j] = _data[j][i];
          for (int i = 0; i < N; i++)
            _data[i].copy_from(tmp2[i], stdx::element_aligned);
          return *this;
        }

        constexpr static Matr<T, N> Id() {
          return {};
        }

        friend std::ostream & operator<<(std::ostream &s, const Matr &m) noexcept {
          for (int i = 0; i < N; i++)
            std::cout << m._data[i] << std::endl;
          return s;
        }
    };
}

#endif // __matr_hpp_
