#ifndef __matr_hpp_
#define __matr_hpp_

#include "def.hpp"

namespace mr {
  template<typename T, std::size_t N>
    class Vec;
  template<typename T, std::size_t N>
    class Matr;

  template<typename T>
    using Matr4 = Matr<T, 4>;

  using Matr4f = Matr4<float>;
  using Matr4d = Matr4<double>;
  using Matr4i = Matr4<int>;
  using Matr4u = Matr4<uint>;

  template<typename T, std::size_t N>
    class Matr {
      public:
        constexpr static inline std::size_t size = N;

      private:
        std::array<stdx::fixed_size_simd<T, N>, N> _data;

      public:
        Matr() = default;

        template<class ... Args> 
          Matr(Args... args) {
            _data = std::array<T, N>(args...);
          }

        Matr(const std::array<std::array<T, N>, N> &arr) {
          for (int i = 0; i < N; i++)
            _data[i].copy_from(&arr[i][0], stdx::element_aligned);
        }

        // template<typename... Args>
        //   Matr(Args... args) {
        //     _data = {args...};
        //   }

        // copy semantics
        Matr(const Matr &other) noexcept = default;
        Matr & operator=(const Matr &other) noexcept = default;

        // move semantics
        Matr(Matr &&other) noexcept = default;
        Matr & operator=(Matr &&other) noexcept = default;

        [[nodiscard]] constexpr Matr operator*=(const Matr &other) noexcept {
          return *this;
        }

        [[nodiscard]] constexpr Matr operator+=(const Matr &other) noexcept {
          std::array<stdx::fixed_size_simd<T, N>, N> tmp;
          for (int i = 0; i < N; i++)
            _data[i] += other._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr Matr operator-=(const Matr &other) noexcept {
          std::array<stdx::fixed_size_simd<T, N>, N> tmp;
          for (int i = 0; i < N; i++)
            _data[i] -= other._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr Matr operator*(const Matr &other) const noexcept {
        }

        [[nodiscard]] constexpr Matr operator+(const Matr &other) const noexcept {
          std::array<stdx::fixed_size_simd<T, N>, N> tmp;
          for (int i = 0; i < N; i++)
            tmp[i] = _data[i] + other._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr Matr operator-(const Matr &other) const noexcept {
          std::array<stdx::fixed_size_simd<T, N>, N> tmp;
          for (int i = 0; i < N; i++)
            tmp[i] = _data[i] - other._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr T operator!() const noexcept {
          return {};
        }

        friend std::ostream & operator<<(std::ostream &s, const Matr &m) noexcept {
          for (int j = 0; j < N; j++)
          {
            s << '(';
            for (int i = 0; i < N; i++)
              s << m._data[j][i] 
                << (char)(',' * (i < N - 1)) 
                << (char)(' ' * (i < N - 1));
            s << ')' << std::endl;
          }
          return s;
        }
    };
}

#endif // __matr_hpp_
