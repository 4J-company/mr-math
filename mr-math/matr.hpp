#ifndef __Matr_hpp_
#define __Matr_hpp_

#include "def.hpp"
#include "row.hpp"
#include "units.hpp"
#include "operators/matr.hpp"

namespace mr {
  // forward declarations
  template <ArithmeticT T, std::size_t N>
    struct Matr;

  // common aliases
  template <ArithmeticT T>
    using Matr3 = Matr<T, 3>;

  using Matr3f = Matr3<float>;
  using Matr3d = Matr3<double>;
  using Matr3i = Matr3<int>;
  using Matr3u = Matr3<uint32_t>;

  template <ArithmeticT T>
    using Matr4 = Matr<T, 4>;

  using Matr4f = Matr4<float>;
  using Matr4d = Matr4<double>;
  using Matr4i = Matr4<int>;
  using Matr4u = Matr4<uint32_t>;

  template <ArithmeticT T, std::size_t N>
    struct [[nodiscard]] Matr : MatrOperators<Matr<T, N>> {
    private:
      using MatrOperators<Matr<T, N>>::_identity;
      static constexpr T _epsilon = std::numeric_limits<T>::epsilon();

    public:
      using ValueT = T;
      inline static constexpr std::size_t size = N;
      using RowT = Row<T, N>;

      inline static const Matr identity = _identity();
      std::array<RowT, N> _data;

      template <typename... Args>
        requires (std::is_same_v<Args, RowT> && ...) &&
                 ((sizeof...(Args) == 0) || (sizeof...(Args) == N))
        constexpr Matr(Args... args) noexcept {
          _data = std::array<RowT, N>({args...});
        }

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

      constexpr Matr(const std::array<RowT, N> &arr) : _data(arr) {}
      constexpr Matr(std::array<RowT, N> &&arr) : _data(std::move(arr)) {}

      // copy semantics
      constexpr Matr(const Matr &) noexcept = default;
      constexpr Matr & operator=(const Matr &) noexcept = default;

      // move semantics
      constexpr Matr(Matr &&) noexcept = default;
      constexpr Matr & operator=(Matr &&) noexcept = default;

      constexpr ~Matr() = default;

      [[nodiscard]] constexpr const RowT &
      operator[](size_t i) const noexcept {
        return _data[i];
      }

      [[nodiscard]] constexpr T determinant() const noexcept {
        std::array<RowT, N> tmp = _data;
        T det = 1;

        tmp[N - 1] /= tmp[N - 1][N - 1];
        for (size_t i = 1; i < N; i++) {
          det *= tmp[i - 1][i - 1];
          tmp[i - 1] /= std::abs(tmp[i - 1][i - 1]) <= _epsilon ? static_cast<T>(1) : tmp[i - 1][i - 1];
          for (size_t j = i; j < N; j++) {
            tmp[i] -= tmp[i - 1] * tmp[j][i - 1];
          }
        }
        return det;
      }

      constexpr Matr<T, N> transposed() const noexcept {
        std::array<RowT, N> tmp1;
        for (size_t i = 0; i < N; i++) {
          tmp1[i] = SimdImpl<T, N>([this, i](auto j){ return _data[j][i]; });
        }
        return {tmp1};
      }

      constexpr Matr<T, N> & transpose() noexcept {
        for (size_t i = 0; i < N; i++) {
          _data[i] = SimdImpl<T, N>([this, i](auto j){ return _data[j][i]; });
        }
        return *this;
      }

      constexpr Matr<T, N> inversed() const noexcept {
        constexpr auto io = std::ranges::iota_view{(size_t)0, N};

        std::array<Row<T, 2 * N>, N> tmp;
        std::for_each(std::execution::par_unseq, io.begin(), io.end(),
                      [&tmp, this](auto i) {
                        // adding temporary variable here brings performance down 2.5 times (reason unknown)
                        tmp[i] += stdx::static_simd_cast<SimdImpl<T, 2 * N>>(stdx::concat(_data[i]._data, identity[i]._data));
                      });

        // null bottom triangle
        for (size_t i = 1; i < N; i++) {
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
        std::for_each(std::execution::par_unseq, io.begin(), io.end(),
          [&tmp, &res](auto i) {
            auto [_, b] = stdx::split<N, N>(tmp[i]._data);
            res[i] = stdx::static_simd_cast<SimdImpl<T, N>>(b);
          });

        return res;
      }

      constexpr Matr<T, N> & inverse() const noexcept {
        *this = inversed();
        return *this;
      }

      static constexpr Matr4<T> rotate(const Radians<T> &rad, const Vec3<T> &vec) noexcept {
        T co = std::cos(rad._data);
        T si = std::sin(rad._data);
        T nco = 1 - co;
        auto v = vec.normalized();

        Matr4<T> tmp1 = scale(v * v * nco + Vec<T, 3>{co});
        Matr4<T> tmp2 = Matr4<T> {
          typename mr::Matr4<T>::RowT(0, v.x() * v.y() * nco, v.x() * v.z() * nco, 0),
          typename mr::Matr4<T>::RowT(v.x() * v.y() * nco, 0, v.y() * v.z() * nco, 0),
          typename mr::Matr4<T>::RowT(v.x() * v.z() * nco, v.y() * v.z() * nco, 0, 0),
          typename mr::Matr4<T>::RowT(0, 0, 0, 0)
        };
        Matr4<T> tmp3 = Matr4<T> {
          typename mr::Matr4<T>::RowT(0, -v.z() * si, v.y() * si, 0),
          typename mr::Matr4<T>::RowT(v.z() * si, 0, -v.x() * si, 0),
          typename mr::Matr4<T>::RowT(-v.y() * si, v.x() * si, 0, 0),
          typename mr::Matr4<T>::RowT(0, 0, 0, 0)
        };

        return tmp1 + tmp2 + tmp3;
      }
    };

  template <ArithmeticT T, std::size_t N> requires (N == 3 || N == 4)
    struct ScaleMatr;

  template <ArithmeticT T>
    struct ScaleMatr<T, 3> : ScaleMatrOperators<ScaleMatr<T, 3>> {
      using MatrT = Matr3<T>;
      using VecT = Vec3<T>;

      VecT scale_vector {};

      ScaleMatr(const std::convertible_to<T> auto scale_factor) :
        scale_vector(scale_factor) {}

      template <std::convertible_to<T> P>
      operator mr::Matr3<P>() {
        return Matr3<P> {
          Matr3<P>::RowT(scale_vector.x(), 0, 0),
          Matr3<P>::RowT(0, scale_vector.y(), 0),
          Matr3<P>::RowT(0, 0, scale_vector.z()),
        };
      }
    };

  template <ArithmeticT T>
    struct ScaleMatr<T, 4> : ScaleMatrOperators<ScaleMatr<T, 4>> {
      using MatrT = Matr4<T>;
      using VecT = Vec4<T>;

      // scale vector
      VecT scale_vector {};

      ScaleMatr(const std::convertible_to<VecT> auto scale_factor) :
        scale_vector(scale_factor) {}

      template <std::convertible_to<T> P>
      operator mr::Matr4<P>() {
        return Matr4<P> {
          Matr4<P>::RowT(scale_vector.x(), 0, 0, 0),
          Matr4<P>::RowT(0, scale_vector.y(), 0, 0),
          Matr4<P>::RowT(0, 0, scale_vector.z(), 0),
          Matr4<P>::RowT(0, 0, 0, scale_vector.w()),
        };
      }
    };

  template <ArithmeticT T>
    struct TranslateMatr4 : TranslateMatrOperators<TranslateMatr4<T>> {
      using MatrT = Matr4<T>;
      using VecT = Vec4<T>;

      // scale vector
      VecT translate_vector {};

      TranslateMatr4(const std::convertible_to<VecT> auto translate_factor) :
        translate_vector(translate_factor) {}

      template <std::convertible_to<T> P>
      operator mr::Matr4<P>() {
        return Matr4<P> {
          Matr4<P>::RowT(1, 0, 0, 0),
          Matr4<P>::RowT(0, 1, 0, 0),
          Matr4<P>::RowT(0, 0, 1, 0),
          translate_vector,
        };
      }
    };

  template <ArithmeticT T, std::size_t N>
    struct RotateXMatr;

  template <ArithmeticT T>
    struct RotateXMatr<T, 3> : MatrOperators<RotateXMatr<T, 3>> {
      using MatrT = Matr4<T>;
      using RadT = Radians<T>;

      RadT rotate_angle;

      RotateXMatr(RadT rotate_angle) : rotate_angle(rotate_angle) {}

      operator MatrT() {
        T co = std::cos(rotate_angle._data);
        T si = std::sin(rotate_angle._data);

        return MatrT {
          typename MatrT::RowT(1, 0, 0),
          typename MatrT::RowT(0, co, si),
          typename MatrT::RowT(0, -si, co),
        };
      }
    };

  template <ArithmeticT T>
    struct RotateXMatr<T, 4> : MatrOperators<RotateXMatr<T, 4>> {
      using MatrT = Matr4<T>;
      using RadT = Radians<T>;

      RadT rotate_angle;

      RotateXMatr(RadT rotate_angle) : rotate_angle(rotate_angle) {}

      operator MatrT() {
        T co = std::cos(rotate_angle._data);
        T si = std::sin(rotate_angle._data);

        return Matr4<T> {
          typename mr::Matr4<T>::RowT(1, 0, 0, 0),
          typename mr::Matr4<T>::RowT(0, co, si, 0),
          typename mr::Matr4<T>::RowT(0, -si, co, 0),
          typename mr::Matr4<T>::RowT(0, 0, 0, 1)
        };
      }
    };

  template <ArithmeticT T, std::size_t N>
    struct RotateYMatr;

  template <ArithmeticT T>
    struct RotateYMatr<T, 3> : MatrOperators<RotateYMatr<T, 3>> {
      using MatrT = Matr3<T>;
      using RadT = Radians<T>;

      RadT rotate_angle;

      RotateYMatr(RadT rotate_angle) : rotate_angle(rotate_angle) {}

      operator MatrT() {
        T co = std::cos(rotate_angle._data);
        T si = std::sin(rotate_angle._data);

        return MatrT {
          typename MatrT::RowT(co, 0, -si),
          typename MatrT::RowT(0, 1, 0),
          typename MatrT::RowT(si, 0, co),
        };
      }
    };

  template <ArithmeticT T>
    struct RotateYMatr<T, 4> : MatrOperators<RotateYMatr<T, 4>> {
      using MatrT = Matr4<T>;
      using RadT = Radians<T>;

      RadT rotate_angle;

      RotateYMatr(RadT rotate_angle) : rotate_angle(rotate_angle) {}

      operator MatrT() {
        T co = std::cos(rotate_angle._data);
        T si = std::sin(rotate_angle._data);

        return MatrT {
          typename MatrT::RowT(co, 0, -si, 0),
          typename MatrT::RowT(0, 1, 0, 0),
          typename MatrT::RowT(si, 0, co, 0),
          typename MatrT::RowT(0, 0, 0, 1)
        };
      }
    };

  template <ArithmeticT T, std::size_t N>
    struct RotateZMatr;

  template <ArithmeticT T>
    struct RotateZMatr<T, 3> : MatrOperators<RotateZMatr<T, 3>> {
      using MatrT = Matr3<T>;
      using RadT = Radians<T>;

      RadT rotate_angle;

      RotateZMatr(RadT rotate_angle) : rotate_angle(rotate_angle) {}

      operator MatrT() {
        T co = std::cos(rotate_angle._data);
        T si = std::sin(rotate_angle._data);

        return MatrT {
          typename MatrT::RowT(co, si, 0),
          typename MatrT::RowT(-si, co, 0),
          typename MatrT::RowT(0, 0, 1),
        };
      }
    };

  template <ArithmeticT T>
    struct RotateZMatr<T, 4> : MatrOperators<RotateZMatr<T, 4>> {
      using MatrT = Matr4<T>;
      using RadT = Radians<T>;

      RadT rotate_angle;

      RotateZMatr(RadT rotate_angle) : rotate_angle(rotate_angle) {}

      operator MatrT() {
        T co = std::cos(rotate_angle._data);
        T si = std::sin(rotate_angle._data);

        return MatrT {
          typename MatrT::RowT(co, si, 0, 0),
          typename MatrT::RowT(-si, co, 0, 0),
          typename MatrT::RowT(0, 0, 1, 0),
          typename MatrT::RowT(0, 0, 0, 1)
        };
      }
    };

  template <typename T>
      static constexpr Matr4<T> rotate(const Radians<T> &rad, const Vec3<T> &vec) noexcept {
        T co = std::cos(rad._data);
        T si = std::sin(rad._data);
        T nco = 1 - co;
        auto v = vec.normalized();

        Matr4<T> tmp1 = scale(v * v * nco + Vec<T, 3>{co});
        Matr4<T> tmp2 = Matr4<T> {
          typename mr::Matr4<T>::RowT(0, v.x() * v.y() * nco, v.x() * v.z() * nco, 0),
          typename mr::Matr4<T>::RowT(v.x() * v.y() * nco, 0, v.y() * v.z() * nco, 0),
          typename mr::Matr4<T>::RowT(v.x() * v.z() * nco, v.y() * v.z() * nco, 0, 0),
          typename mr::Matr4<T>::RowT(0, 0, 0, 0)
        };
        Matr4<T> tmp3 = Matr4<T> {
          typename mr::Matr4<T>::RowT(0, -v.z() * si, v.y() * si, 0),
          typename mr::Matr4<T>::RowT(v.z() * si, 0, -v.x() * si, 0),
          typename mr::Matr4<T>::RowT(-v.y() * si, v.x() * si, 0, 0),
          typename mr::Matr4<T>::RowT(0, 0, 0, 0)
        };

        return tmp1 + tmp2 + tmp3;
      }
}

#ifdef __cpp_lib_format
// std::format support
namespace std {
  template <mr::ArithmeticT T, size_t N>
    struct formatter<mr::Matr<T, N>> {
      template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx) {
          // skip all format specifiers
          return ctx.end();
        }

      template<typename FmtContext>
        auto format(const mr::Matr<T, N> &m, FmtContext& ctx) const {
          ostringstream out;
          out << '(' << m[0] << ",\n";
          for (size_t i = 1; i < N - 1; i++)
            out << ' ' << m[i] << ",\n";
          out << ' ' << m[N - 1] << ')';

          return ranges::copy(std::move(out).str(), ctx.out()).out;
        }
    };
} // namespace std
#endif

#endif // __Matr_hpp_
