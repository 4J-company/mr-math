#ifndef __matr_hpp_
#define __matr_hpp_

#include "def.hpp"

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
    class [[nodiscard]] Matr {
      public:
        constexpr static inline std::size_t size = N;

        struct Row : stdx::fixed_size_simd<T, N> {
        public:
          Row(const T *data) {
            stdx::fixed_size_simd<T, N>::copy_from(data, stdx::element_aligned);
          }

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

        Matr(const std::array<Row, N> &arr) {
          _data = arr;
        }

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

        constexpr Matr operator*=(const Matr &other) noexcept {
          const Matr<T, N> tr = other.transposed();
          for (int i = 0; i < N; i++)
            _data[i] *= tr._data[i];
          return *this;
        }

        constexpr Matr operator+=(const Matr &other) noexcept {
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            _data[i] += other._data[i];
          return {tmp};
        }

        constexpr Matr operator-=(const Matr &other) noexcept {
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            _data[i] -= other._data[i];
          return {tmp};
        }

        constexpr Matr operator*(const Matr &other) const noexcept {
          const Matr<T, N> tr = other.transposed();
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            tmp[i] = _data[i] + tr._data[i];
          return {tmp};
        }

        constexpr Matr operator+(const Matr &other) const noexcept {
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            tmp[i] = _data[i] + other._data[i];
          return {tmp};
        }

        constexpr Matr operator-(const Matr &other) const noexcept {
          std::array<Row, N> tmp;
          for (int i = 0; i < N; i++)
            tmp[i] = _data[i] - other._data[i];
          return {tmp};
        }

        [[nodiscard]] constexpr T determinant() const noexcept {
          std::array<Row, N> tmp = _data;

          /// impossible because of lack of operator= for std::experemental::simd
          //
          // auto io1 = std::ranges::iota_view((size_t)1, N);
          // std::transform(io1.begin(), io1.end(), tmp.begin(),
          //     [&tmp](size_t i) {
          //     return (tmp[i] - (tmp[i][i - 1] == 0 ? 0 : tmp[i - 1][i - 1] /  tmp[i][i - 1]));
          //

          for (int i = 1; i < N; i++) {
            tmp[i] -= tmp[i][i - 1] == 0 ? 0 : tmp[i - 1][i - 1] /  tmp[i][i - 1];
          }

          auto io0 = std::ranges::iota_view((size_t)0, N);
          return std::transform_reduce(
              std::execution::par_unseq,
              io0.begin(), io0.end(), 1,
              std::multiplies {},
              [&tmp](auto i) {
                return tmp[i][i];
              }
              );
        }

        [[nodiscard]] constexpr T operator!() const noexcept {
          return determinant();
        }

        constexpr Matr transposed() const noexcept {
          std::array<Row, N> tmp1;
          std::array<std::array<T, N>, N> tmp2;
          for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
              tmp2[i][j] = _data[j][i];
          for (int i = 0; i < N; i++)
            tmp1[i].copy_from(tmp2[i], stdx::element_aligned);
          return {tmp1};
        }

        constexpr Matr & transpose() noexcept {
          std::array<std::array<T, N>, N> tmp2;
          for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
              tmp2[i][j] = _data[j][i];
          for (int i = 0; i < N; i++)
            _data[i].copy_from(tmp2[i], stdx::element_aligned);
          return *this;
        }

        constexpr static Matr Id() {
          std::array<Row, N> tmp;
          auto io = std::ranges::iota_view{(size_t)0, N};

          std::transform(
              std::execution::par_unseq,
              io.begin(), io.end(), tmp.begin(),
              [&io](auto i) -> Row {
                std::array<T, N> tmp;
                std::transform(
                    std::execution::par_unseq,
                    io.begin(), io.end(), tmp.begin(),
                    [i](auto i2) -> T {
                      return i2 == i ? 1 : 0;
                    });
                return {tmp.data()};
              });

          return tmp;
        }

        friend std::ostream & operator<<(std::ostream &s, const Matr &m) noexcept {
          for (int i = 0; i < N; i++)
            std::cout << m._data[i] << std::endl;
          return s;
        }
    };
}

#endif // __matr_hpp_
