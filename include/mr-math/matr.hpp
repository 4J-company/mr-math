#ifndef __MR_MATR_HPP_
#define __MR_MATR_HPP_

#include "def.hpp"
#include "row.hpp"
#include "units.hpp"

namespace mr
{
  // forward declarations
  template <ArithmeticT T, std::size_t N>
    struct Matr;

  // common aliases
  template <ArithmeticT T>
    using Matr4 = Matr<T, 4>;

  using Matr4f = Matr4<float>;
  using Matr4d = Matr4<double>;
  using Matr4i = Matr4<int>;
  using Matr4u = Matr4<uint32_t>;

  template <ArithmeticT T, std::size_t N>
    struct [[nodiscard]] Matr
    {
    public:
      using ValueT = T;
      using RowT = Row<T, N>;

      constexpr Matr() noexcept = default;

      template <typename... Args>
        requires (std::is_same_v<Args, RowT> && ...) &&
                 (sizeof...(Args) == N)
        constexpr Matr(Args... args) noexcept {
          _data = std::array<RowT, N>({args...});
        }

      constexpr Matr(std::array<RowT, N> rows) : _data(std::move(rows)) {}

      template <typename... Args>
        requires (std::is_convertible_v<Args, T> && ...) &&
                 (sizeof...(Args) == N * N)
        constexpr Matr(Args... args) noexcept {
          std::array<T, N * N> tmp {static_cast<T>(args)...};
          for (size_t i = 0; i < N; i++) {
            _data[i] = RowT {
              tmp[N * i + 0],
              tmp[N * i + 1],
              tmp[N * i + 2],
              tmp[N * i + 3]
            };
          }
        }

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
            tmp[j] += other._data[i] * _data[j][i];
          }
        }
        *this = tmp;
        return *this;
      }

      constexpr Matr & operator+=(const Matr &other) noexcept {
        for (int i = 0; i < N; i++) {
          _data[i] += other._data[i];
        }
        return *this;
      }

      constexpr Matr & operator-=(const Matr &other) noexcept {
        for (int i = 0; i < N; i++) {
          _data[i] -= other._data[i];
        }
        return *this;
      }

      constexpr Matr operator*(const Matr &other) const noexcept {
        std::array<RowT, N> tmp{};
        for (size_t i = 0; i < N; i++) {
          for (size_t j = 0; j < N; j++) {
            tmp[j] += other._data[i] * _data[j][i];
          }
        }
        return {tmp};
      }

      constexpr Matr operator+(const Matr &other) const noexcept {
        std::array<RowT, N> tmp;
        for (size_t i = 0; i < N; i++) {
          tmp[i] = static_cast<RowT>(_data[i] + other._data[i]);
        }
        return {tmp};
      }

      constexpr Matr operator-(const Matr &other) const noexcept {
        std::array<RowT, N> tmp;
        for (size_t i = 0; i < N; i++) {
          tmp[i] = _data[i] - other._data[i];
        }
        return {tmp};
      }

      // matrix related operations
      [[nodiscard]] constexpr const RowT & operator[](size_t i) const noexcept {
        return _data[i];
      }

      [[nodiscard]] constexpr RowT & operator[](size_t i) noexcept {
        return _data[i];
      }

      // TODO: fix (or remove as it's neved used)
      [[nodiscard]] constexpr T determinant() const noexcept {
        std::array<RowT, N> tmp = _data;
        T det = 1;

        tmp[N - 1] /= tmp[N - 1][N - 1];
        for (size_t i = 0; i < N - 1; i++) {
          det *= tmp[i][i];
          tmp[i] /= mr::equal(tmp[i][i], 0) ? static_cast<T>(1) : tmp[i][i];
          for (size_t j = i + 1; j < N; j++) {
            tmp[i + 1] -= tmp[i] * tmp[j][i];
          }
        }
        return det;
      }

      constexpr Matr transposed() const noexcept {
        Matr transposed = *this;
        transposed.transpose();
        return transposed;
        // Matr transposed;
        // for (size_t i = 0; i < N; i++) {
        //   transposed[i] = SimdImpl<T, N>([this, i](size_t j) { return _data[j][i]; });
        // }
        // return transposed;
      }

      constexpr Matr & transpose() noexcept {
        auto *ptr = &_data.front()._data;
        stdx::transpose(ptr, ptr + N);
        return *this;
        // *this = transposed();
        // return *this;
      }

      constexpr Matr inversed() const noexcept {
        constexpr auto io = std::ranges::iota_view{(size_t)0, N};

        struct Row2N {
          std::array<Row<T, N>, 2> rows;

          Row2N(T value = 0) {
            rows[0] = {0};
            rows[1] = {0};
          }

          void load(const std::array<T, 2 * N> &buf) {
            rows[0] = stdx::load_unaligned(buf.data());
            rows[1] = stdx::load_unaligned(buf.data() + N);
          }

          void store(std::array<T, 2 * N> &buf) {
            rows[0]._data.store_unaligned(buf.data());
            rows[1]._data.store_unaligned(buf.data() + N);
          }

          Row2N operator*(T value) {
            Row2N res;
            res.rows[0] = rows[0] * value;
            res.rows[1] = rows[1] * value;
            return res;
          }

          Row2N & operator/=(T value) {
            rows[0] /= value;
            rows[1] /= value;
            return *this;
          }

          Row2N & operator-=(const Row2N &other) {
            rows[0] -= other.rows[0];
            rows[1] -= other.rows[1];
            return *this;
          }

          T operator[](std::size_t i) {
            return i < N ? rows[0][i] : rows[1][i - N];
          }
        };

        std::array<Row2N, N> tmp {};
        for (auto i : io) {
          std::array<T, 2 * N> buf;
          _data[i]._data.store_unaligned(buf.data());
          _identity[i]._data.store_unaligned(buf.data() + N);
          tmp[i].load(buf);
        }

        for (std::size_t i = 1; i < N; i++) {
          tmp[i - 1] /= tmp[i - 1][i - 1];
          for (size_t j = i; j < N; j++) {
            tmp[j] -= std::abs(tmp[i - 1][i - 1]) <= _epsilon ? static_cast<T>(0) : tmp[i - 1] * tmp[j][i - 1];
          }
        }
        tmp[N - 1] /= tmp[N - 1][N - 1];

        // null top triangle
        for (int i = N - 2; i >= 0; i--) {
          for (int j = i; j >= 0; j--) {
            tmp[j] -= std::abs(tmp[i + 1][i + 1]) <= _epsilon ? static_cast<T>(0) : tmp[i + 1] * tmp[j][i + 1];
          }
        }

        std::array<RowT, N> res;
        for (auto i : io) {
          std::array<T, 2 * N> buf;
          tmp[i].store(buf);
          res[i]._data = stdx::load_unaligned(buf.data() + N);
        }

        return res;
      }

      constexpr Matr & inverse() noexcept {
        *this = inversed();
        return *this;
      }

      static constexpr Matr identity() noexcept {
        return _identity;
      }

      static constexpr Matr4<T> scale(const Vec3<T> &vec) noexcept {
        return Matr4<T> {
          vec.x(),       0,       0, 0,
                0, vec.y(),       0, 0,
                0,       0, vec.z(), 0,
                0,       0,       0, 1
        };
      }

      static constexpr Matr4<T> translate(const Vec3<T> &vec) noexcept {
        return Matr4<T> {
                1,       0,       0, 0,
                0,       1,       0, 0,
                0,       0,       1, 0,
          vec.x(), vec.y(), vec.z(), 1
        };
      }

      static constexpr Matr4<T> rotate_x(const Radians<T> &rad) noexcept {;
        T co = std::cos(rad._data);
        T si = std::sin(rad._data);

        if (axis::x.x() < 0) {
          return Matr4<T> {
            1,  0,   0, 0,
            0, co, -si, 0,
            0, si,  co, 0,
            0,  0,   0, 1
          };
        }

        return Matr4<T> {
          1,   0,  0, 0,
          0,  co, si, 0,
          0, -si, co, 0,
          0,   0,  0, 1
        };
      }

      static constexpr Matr4<T> rotate_y(const Radians<T> &rad) noexcept {
        T co = std::cos(rad._data);
        T si = std::sin(rad._data);
        if (axis::y.y() < 0) {
          return Matr4<T> {
             co, 0, si, 0,
              0, 1,  0, 0,
            -si, 0, co, 0,
              0, 0,  0, 1
          };
        }

        return Matr4<T> {
          co, 0, -si, 0,
           0, 1,   0, 0,
          si, 0,  co, 0,
           0, 0,   0, 1
        };
      }

      static constexpr Matr4<T> rotate_z(const Radians<T> &rad) noexcept {
        T co = std::cos(rad._data);
        T si = std::sin(rad._data);

        // this is required for passing a test with our basis (z = {0, 0, -1})
        // TODO: consider possibility of swapping axes (maybe we should change these matrices when basis is set or just use quaternions)
        if (axis::z.z() < 0) {
          return Matr4<T> {
            co, -si, 0, 0,
            si,  co, 0, 0,
             0,   0, 1, 0,
             0,   0, 0, 1
          };
        }

        return Matr4<T> {
           co, si, 0, 0,
          -si, co, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        };
      }

      static constexpr Matr4<T> rotate(const Norm<T, 3> &n, const Radians<T> &rad) noexcept {
        T co = std::cos(rad._data);
        T si = std::sin(rad._data);
        T nco = 1 - co;

        Matr4<T> tmp1 = scale(n * n * nco + Vec<T, 3>{co});
        Matr4<T> tmp2 = Matr4<T> {
                            0, n.x() * n.y() * nco, n.x() * n.z() * nco, 0,
          n.x() * n.y() * nco,                   0, n.y() * n.z() * nco, 0,
          n.x() * n.z() * nco, n.y() * n.z() * nco,                   0, 0,
                            0,                   0,                   0, 0
        };
        Matr4<T> tmp3 = Matr4<T> {
                    0, -n.z() * si,  n.y() * si, 0,
           n.z() * si,           0, -n.x() * si, 0,
          -n.y() * si,  n.x() * si,           0, 0,
                    0,           0,           0, 0
        };

        return tmp1 + tmp2 + tmp3;
      }

      constexpr bool operator==(const Matr &other) const noexcept {
        for (size_t i = 0; i < N; i++) {
          if (_data[i] != other._data[i]) {
            return false;
          }
        }
        return true;
      }

      constexpr bool equal(const Matr &other, ValueT eps = epsilon<ValueT>()) const noexcept {
        for (size_t i = 0; i < N; i++) {
          if (not _data[i].equal(other._data[i], eps)) {
            return false;
          }
        }
        return true;
      }

      friend std::ostream & operator<<(std::ostream &os, const Matr &m) noexcept {
        os << "\n(" << m[0] << ",\n";
          for (size_t i = 1; i < N - 1; i++)
            os << ' ' << m[i] << ",\n";
          os << ' ' << m[N - 1] << ')';
        return os;
      }

    private:
      static Matr get_identity() {
        // TODO: why it doesnt work?
        // std::array<RowT, N> id {
        //   {1, 0, 0, 0},
        //   {0, 1, 0, 0},
        //   {0, 0, 1, 0},
        //   {0, 0, 0, 1}
        // };

        std::array<RowT, N> id;
        id[0] = {1, 0, 0, 0};
        id[1] = {0, 1, 0, 0};
        id[2] = {0, 0, 1, 0};
        id[3] = {0, 0, 0, 1};

        // std::array<RowT, N> id;
        // constexpr auto io = std::ranges::iota_view {(size_t)0, N};

        // std::transform(
        //   io.begin(), io.end(), id.begin(),
        //   [&io](size_t i) -> RowT {
        //     return SimdImpl<T, N>([i](size_t j) { return j == i ? 1 : 0; });
        //   });

        return id;
      }

    public:
      std::array<RowT, N> _data;

    private:
      static const Matr _identity;
      inline static const T _epsilon = std::numeric_limits<T>::epsilon();
    };

    // this is required to initialize 'Matr::_identity' on MSVC 
    template <ArithmeticT T, std::size_t N>
      const Matr<T, N> Matr<T, N>::_identity = Matr<T, N>::get_identity();

} // namespace mr

#ifdef __cpp_lib_format
// std::format support
namespace std {
  template <mr::ArithmeticT T, size_t N>
    struct formatter<mr::Matr<T, N>> {
      template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx) {
          // skip all format specifiers
          auto it = ctx.begin();
          while (*it != '}')
              ++it;
          return it;
        }

      template<typename FmtContext>
        auto format(const mr::Matr<T, N> &m, FmtContext& ctx) const {
          ostringstream out;
          out << m;
          return ranges::copy(std::move(out).str(), ctx.out()).out;
        }
    };
} // namespace std
#endif

#endif // __MR_MATR_HPP_
