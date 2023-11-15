#ifndef __Matr_hpp_
#define __Matr_hpp_

#include "def.hpp"
#include "row.hpp"

namespace mr
{
  // forward declarations
  template <ArithmeticT T, std::size_t N>
    class Matr;

  template <ArithmeticT T>
    class Matr4;

  // common aliases
  using Matr4f = Matr4<float>;
  using Matr4d = Matr4<double>;
  using Matr4i = Matr4<int>;
  using Matr4u = Matr4<uint_t>;

  template <ArithmeticT T, std::size_t N>
    class [[nodiscard]] Matr
    {
    public:
      using RowT = Row<T, N>;

      template <typename... Args>
        requires (std::is_same_v<Args, RowT> && ...) &&
                 ((sizeof...(Args) == 0) || (sizeof...(Args) == N))
        constexpr Matr(Args... args) noexcept {
          _data = std::array<RowT, N>({args...});
        }

      constexpr Matr(const std::array<RowT, N> &arr) : _data(arr) {}

      // copy semantics
      constexpr Matr(const Matr &) noexcept = default;
      constexpr Matr & operator=(const Matr &) noexcept = default;

      // move semantics
      constexpr Matr(Matr &&) noexcept = default;
      constexpr Matr & operator=(Matr &&) noexcept = default;

      constexpr ~Matr() = default;

      // basic math operations
      constexpr Matr & operator*=(const Matr &other) noexcept {
        std::array<RowT, N> tmp {};
        for (size_t i = 0; i < N; i++) {
          for (size_t j = 0; j < N; j++) {
            tmp._data[j] += other._data[i] * _data[j][i];
          }
        }
        *this = tmp;
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
        std::array<RowT, N> tmp;
        for (size_t i = 0; i < N; i++) {
          for (size_t j = 0; j < N; j++) {
            tmp[j] += other._data[i] * _data[j][i];
          }
        }
        return {tmp};
      }

      constexpr Matr operator+(const Matr &other) const noexcept {
        std::array<RowT, N> tmp;
        for (size_t i = 0; i < N; i++)
          tmp[i] = static_cast<RowT>(_data[i] + other._data[i]);
        return {tmp};
      }

      constexpr Matr operator-(const Matr &other) const noexcept {
        std::array<RowT, N> tmp;
        for (size_t i = 0; i < N; i++)
          tmp[i] = _data[i] - other._data[i];
        return {tmp};
      }

      // matrix related operations
      [[nodiscard]] constexpr T determinant() const noexcept {
        std::array<RowT, N> tmp = _data;

        for (size_t i = 1; i < N; i++) {
          for (size_t j = i; j < N; j++) {
            tmp[i] -= tmp[i][i - 1] == 0
              ? 0
              : tmp[i - 1] * tmp[j][i - 1] / tmp[i - 1][i - 1];
          }
        }

        constexpr auto io0 = std::ranges::iota_view((size_t)0, N);
        return std::transform_reduce(
          std::execution::par_unseq,
          io0.begin(), io0.end(), 1,
          std::multiplies {},
          [&tmp](auto i) { return tmp[i][i]; });
      }

      [[nodiscard]] constexpr T operator!() const noexcept {
        return determinant();
      }

      constexpr Matr transposed() const noexcept {
        std::array<RowT, N> tmp1;
        std::array<std::array<T, N>, N> tmp2;
        for (size_t i = 0; i < N; i++)
          for (size_t j = 0; j < N; j++)
            tmp2[i][j] = _data[j][i];
        for (size_t i = 0; i < N; i++)
          tmp1[i]._data.copy_from(tmp2[i].data(), stdx::element_aligned);
        return {tmp1};
      }

      constexpr Matr & transpose() noexcept {
        std::array<std::array<T, N>, N> tmp2;
        for (size_t i = 0; i < N; i++)
          for (size_t j = 0; j < N; j++)
            tmp2[i][j] = _data[j][i];
        for (size_t i = 0; i < N; i++)
          _data[i].copy_from(tmp2[i].data(), stdx::element_aligned);
        return *this;
      }

      constexpr Matr inversed() const noexcept {
        constexpr auto io = std::ranges::iota_view{(size_t)0, N};

        std::array<Row<T, 2 * N>, N> tmp;
        Matr id = identity();
        std::for_each(std::execution::par_unseq, io.begin(), io.end(),
                      [&tmp, &id, this](auto i) { tmp[i] += stdx::concat(_data[i], id[i]); });

        // null bottom triangle
        for (size_t i = 1; i < N; i++) {
          for (size_t j = i; j < N; j++) {
            tmp[j] -= tmp[i - 1][i - 1] == 0 ? 0 : tmp[i - 1] * tmp[j][i - 1] / tmp[i - 1][i - 1];
          }
        }

        // null top triangle
        for (int i = N - 2; i >= 0; i--) {
          for (int j = i; j >= 0; j--) {
            tmp[j] -= tmp[i + 1][i + 1] == 0 ? 0 : tmp[i + 1] * tmp[j][i + 1] / tmp[i + 1][i + 1];
          }
        }

        // make main diagonal 1
        std::for_each(std::execution::par_unseq,
          io.begin(), io.end(), [&tmp](auto i) { tmp[i] /= tmp[i][i]; });

        std::array<RowT, N> res;
        std::for_each(std::execution::par_unseq, io.begin(), io.end(),
          [&tmp, &res](auto i) {
            auto [a, b] = stdx::split<N, N>(tmp[i]._data);
            res[i] += stdx::simd_cast<stdx::fixed_size_simd<T, N>>(b);
          });

        return {res};
      }

      constexpr Matr & inverse() const noexcept {
        *this = inversed();
        return *this;
      }

      friend std::ostream & operator<<(std::ostream &s, const Matr &m) noexcept {
        for (size_t i = 0; i < N; i++)
          std::cout << m._data[i] << std::endl;
        return s;
      }

    private:
      static Matr _identity() {
        std::array<RowT, N> id;
        constexpr auto io = std::ranges::iota_view {(size_t)0, N};

        std::transform(std::execution::par_unseq,
          io.begin(), io.end(), id.begin(),
          [&io](auto i) -> RowT {
            std::array<T, N> tmp;
            std::transform(std::execution::par_unseq,
              io.begin(), io.end(), tmp.begin(),
              [i](auto i2) -> T { return i2 == i ? 1 : 0; });
            return {tmp.data()};
          });

        return {id};
      }

    public:
      inline static const Matr identity = _identity();
      std::array<RowT, N> _data;
    };

  // specialization for special 4x4 matrices
  template <ArithmeticT T>
    class [[nodiscard]] Matr4 : public Matr<T, 4>
    {
    private:
      using MatrT = Matr<T, 4>;

    public:
      using RowT = MatrT::RowT;

      // constructors
      constexpr Matr4(RowT r0, RowT r1, RowT r2, RowT r3) noexcept
        : MatrT(r0, r1, r2, r3) {}

      constexpr Matr4(const std::array<RowT, 4> &arr) : MatrT(arr) {}

      // copy semantics
      constexpr Matr4(const Matr4 &) noexcept = default;
      constexpr Matr4 & operator=(const Matr4 &) noexcept = default;

      // move semantis
      constexpr Matr4(Matr4 &&) noexcept = default;
      constexpr Matr4 & operator=(Matr4 &&) noexcept = default;

      constexpr ~Matr4() = default;

      // special matrices
      static constexpr Matr4 view(const Vec3<T> &position, const Vec3<T> &direction,
                                  const Vec3<T> &up, const Vec3<T> right) noexcept {
        return {
          RowT{          right.x(),           up.x(),          direction.x(), 0},
          RowT{          right.y(),           up.y(),          direction.y(), 0},
          RowT{          right.z(),           up.z(),          direction.z(), 0},
          RowT{-(position & right), -(position & up), (position & direction), 1}};
      }

      static constexpr Matr4 orthogonal(T r, T l, T t, T b, T n, T f) noexcept {
        return {
          RowT{      2 / (r - l),                 0,                 0, 0},
          RowT{                0,       2 / (t - b),                 0, 0},
          RowT{                0,                 0,       2 / (n - f), 0},
          RowT{(r + l) / (l - r), (t + b) / (b - t), (f + n) / (n - f), 1}};
      }

      static constexpr Matr4 frutstum(T r, T l, T t, T b, T n, T f) noexcept {
        return {
          RowT{  2 * n / (r - l),                 0,                   0,  0},
          RowT{                0,   2 * n / (t - b),                   0,  0},
          RowT{(r + l) / (r - l), (t + b) / (t - b),   (f + n) / (n - f), -1},
          RowT{                0,                 0, 2 * n * f / (n - f),  0}};
      }
    };
} // namespace mr

#endif // __Matr_hpp_
