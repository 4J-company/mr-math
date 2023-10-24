#ifndef __Matr_hpp_
#define __Matr_hpp_

#include "def.hpp"
#include "row.hpp"

namespace mr {
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
      using Row_t = Row<T, N>;

    private:
      inline static std::array<Row<T, N>, N> id;
      inline static std::once_flag id_calculated_flag;

      std::array<Row_t, N> _data;

    public:
      Matr() = default;

      Matr(const std::array<Row_t, N> &arr) {
        _data = arr;
      }

      template <
        typename ... Args
      > requires (sizeof...(Args) == N) && (std::same_as<Args, Row<T, N>> && ...)
      Matr(Args ...args) {
        _data = std::array<Row_t, N>({static_cast<Row_t>(args)...});
      }

      // copy semantics
      Matr(const Matr &other) noexcept = default;
      Matr & operator=(const Matr &other) noexcept = default;

      // move semantics
      Matr(Matr &&other) noexcept = default;
      Matr & operator=(Matr &&other) noexcept = default;

      constexpr Matr & operator*=(const Matr &other) noexcept {
        const Matr<T, N> tr = other.transposed();
        std::array<std::array<T, N>, N> tmp1;
        for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
            tmp1[i][j] = stdx::reduce(_data[i] * tr._data[j]);
          }
        }
        for (int i = 0; i < N; i++)
          _data[i].copy_from(tmp1[i].data(), stdx::element_aligned);

        return *this;
      }

      constexpr Matr & operator+=(const Matr &other) noexcept {
        for (int i = 0; i < N; i++)
          _data[i] += other._data[i];
        return *this;
      }

      constexpr Matr & operator-=(const Matr &other) noexcept {
        for (int i = 0; i < N; i++)
          _data[i] -= other._data[i];
        return *this;
      }

      constexpr Matr operator*(const Matr &other) const noexcept {
        const Matr<T, N> tr = other.transposed();
        std::array<std::array<T, N>, N> tmp1;
        for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
            tmp1[i][j] = stdx::reduce(_data[i] * tr._data[j]);
          }
        }
        std::array<Row_t, N> tmp2;
        for (int i = 0; i < N; i++)
          tmp2[i].copy_from(tmp1[i].data(), stdx::element_aligned);
        return {tmp2};
      }

      constexpr Matr operator+(const Matr &other) const noexcept {
        std::array<Row_t, N> tmp;
        for (int i = 0; i < N; i++)
          tmp[i] = _data[i] + other._data[i];
        return {tmp};
      }

      constexpr Matr operator-(const Matr &other) const noexcept {
        std::array<Row_t, N> tmp;
        for (int i = 0; i < N; i++)
          tmp[i] = _data[i] - other._data[i];
        return {tmp};
      }

      [[nodiscard]] constexpr T determinant() const {
        std::array<Row_t, N> tmp = _data;

        for (int i = 1; i < N; i++) {
          for (int j = i; j < N; j++) {
            tmp[j] -= tmp[i - 1] * tmp[j][i - 1] /  tmp[i - 1][i - 1];
          }
        }

        constexpr auto io0 = std::ranges::iota_view((size_t)0, N);
        return std::transform_reduce(
            std::execution::par_unseq,
            io0.begin(), io0.end(), 1,
            std::multiplies {},
            [&tmp](auto i) {
            return tmp[i][i];
            }
            );
      }

      [[nodiscard]] constexpr T determinant_safe() const noexcept {
        std::array<Row_t, N> tmp = _data;

        for (int i = 1; i < N; i++) {
          for (int j = i; j < N; j++) {
            tmp[i] -= tmp[i][i - 1] == 0 ? 0 : tmp[i - 1] * tmp[j][i - 1] /  tmp[i - 1][i - 1];
          }
        }

        constexpr auto io0 = std::ranges::iota_view((size_t)0, N);
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
        return determinant_safe();
      }

      constexpr Matr transposed() const noexcept {
        std::array<Row_t, N> tmp1;
        std::array<std::array<T, N>, N> tmp2;
        for (int i = 0; i < N; i++)
          for (int j = 0; j < N; j++)
            tmp2[i][j] = _data[j][i];
        for (int i = 0; i < N; i++)
          tmp1[i].copy_from(tmp2[i].data(), stdx::element_aligned);
        return {tmp1};
      }

      constexpr Matr & transpose() noexcept {
        std::array<std::array<T, N>, N> tmp2;
        for (int i = 0; i < N; i++)
          for (int j = 0; j < N; j++)
            tmp2[i][j] = _data[j][i];
        for (int i = 0; i < N; i++)
          _data[i].copy_from(tmp2[i].data(), stdx::element_aligned);
        return *this;
      }

      constexpr Matr inversed() const {
        constexpr auto io = std::ranges::iota_view((size_t)0, N);
        auto id = Id()._data;

        std::array<Row<T, 2 * N>, N> tmp;
        for (int i = 0; i < N; i++) {
          tmp[i] += stdx::concat(_data[i], id[i]);
        }

        // null bottom triangle
        for (int i = 1; i < N; i++) {
          for (int j = i; j < N; j++) {
            tmp[j] -= tmp[i - 1] * tmp[j][i - 1] /  tmp[i - 1][i - 1];
          }
        }

        // null top triangle
        for (int i = N - 2; i >= 0; i--) {
          for (int j = i; j >= 0; j--) {
            tmp[j] -= tmp[i + 1] * tmp[j][i + 1] /  tmp[i + 1][i + 1];
          }
        }

        // make main diagonal 1
        for (int i = 0; i < N; i++) {
          tmp[i] /= tmp[i][i];
        }

        std::array<Row_t, N> res;
        for (int i = 0; i < N; i++) {
          auto [a, b] = stdx::split<N, N>(tmp[i]);
          res[i] += stdx::simd_cast<stdx::fixed_size_simd<T, N>>(b);
        }

        return {res};
      }

      constexpr Matr inversed_safe() const noexcept {
        constexpr auto io = std::ranges::iota_view((size_t)0, N);

        std::array<Row<T, 2 * N>, N> tmp;
        for (int i = 0; i < N; i++) {
          tmp[i] += stdx::concat(_data[i], id[i]);
        }

        // null bottom triangle
        for (int i = 1; i < N; i++) {
          for (int j = i; j < N; j++) {
            tmp[j] -= tmp[i - 1][i - 1] == 0 ? 0 : tmp[i - 1] * tmp[j][i - 1] /  tmp[i - 1][i - 1];
          }
        }

        // null top triangle
        for (int i = N - 2; i >= 0; i--) {
          for (int j = i; j >= 0; j--) {
            tmp[j] -= tmp[i + 1][i + 1] == 0 ? 0 : tmp[i + 1] * tmp[j][i + 1] /  tmp[i + 1][i + 1];
          }
        }

        // make main diagonal 1
        for (int i = 0; i < N; i++) {
          tmp[i] *= tmp[i][i] == 0 ? 1 : 1 / tmp[i][i];
        }

        std::array<Row_t, N> res;
        for (int i = 0; i < N; i++) {
          auto [a, b] = stdx::split<N, N>(tmp[i]);
          res[i] += stdx::simd_cast<stdx::fixed_size_simd<T, N>>(b);
        }

        return {res};
      }

      constexpr Matr & inverse() const {
        *this = inversed();
        return *this;
      }

      constexpr Matr & inverse_safe() const noexcept {
        *this = inversed_safe();
        return *this;
      }

      constexpr static Matr Id() {
        std::call_once(id_calculated_flag, [](auto &tmp) {
            constexpr auto io = std::ranges::iota_view{(size_t)0, N};

            std::transform(
                std::execution::par_unseq,
                io.begin(), io.end(), tmp.begin(),
                [&io](auto i) -> Row_t {
                std::array<T, N> tmp;
                std::transform(
                    std::execution::par_unseq,
                    io.begin(), io.end(), tmp.begin(),
                    [i](auto i2) -> T {
                    return i2 == i ? 1 : 0;
                    });
                return {tmp.data()};
                });
            }, id);

        return {id};
      }

      friend std::ostream & operator<<(std::ostream &s, const Matr &m) noexcept {
        for (int i = 0; i < N; i++)
          std::cout << m._data[i] << std::endl;
        return s;
      }
      };
    }

#endif // __Matr_hpp_
