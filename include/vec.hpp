#ifndef __Vec_hpp_
#define __Vec_hpp_

#include "def.hpp"
#include "row.hpp"

namespace mr
{
  // forward declarations
  template <ArithmeticT T, std::size_t N> requires (N >= 2)
    struct Vec;
  template <ArithmeticT T, std::size_t N>
    class Matr;

  template <ArithmeticT T>
    using Vec2 = Vec<T, 2>;
  template <ArithmeticT T>
    using Vec3 = Vec<T, 3>;
  template <ArithmeticT T>
    using Vec4 = Vec<T, 4>;

  // common aliases
  using Vec2i = Vec2<int>;
  using Vec3i = Vec3<int>;
  using Vec4i = Vec4<int>;

  using Vec2u = Vec2<uint32_t>;
  using Vec3u = Vec3<uint32_t>;
  using Vec4u = Vec4<uint32_t>;

  using Vec2f = Vec2<float>;
  using Vec3f = Vec3<float>;
  using Vec4f = Vec4<float>;

  using Vec2d = Vec2<double>;
  using Vec3d = Vec3<double>;
  using Vec4d = Vec4<double>;

  // base vector (use aliases for full functional)
  template <ArithmeticT T, std::size_t N> requires (N >= 2)
    struct [[nodiscard]] Vec : public Row<T, N, Vec<T, N>>
    {
    public:
      using RowImplT = RowImpl<T, N>;
      using RowT = Row<T, N, Vec<T, N>>;

      // from simd constructor
      constexpr Vec(RowT r) : RowT(r) {};

      // from elements constructor
      template <ArithmeticT... Args>
        constexpr Vec(Args... args) : RowImplT{static_cast<T>(args)...} {}

      // conversion constructor
      template <ArithmeticT R, std::size_t S>
        constexpr Vec(const Vec<R, S> &v) noexcept : RowT(static_cast<RowImpl<R, S>>(v)) {}
      template <ArithmeticT R, std::size_t S, ArithmeticT ... Args>
        constexpr Vec(const Vec<R, S> &v, Args ... args) noexcept : RowT(static_cast<Row<R, S>>(v), args...) {}

      // move semantics
      constexpr Vec(Vec &&) noexcept = default;
      constexpr Vec & operator=(Vec &&) noexcept = default;

      // copy semantics
      constexpr Vec(const Vec &) noexcept = default;
      constexpr Vec & operator=(const Vec &) noexcept = default;

      // setters
      constexpr void x(T x) noexcept requires (N >= 1) { _set_ind(0, x); }
      constexpr void y(T y) noexcept requires (N >= 2) { _set_ind(1, y); }
      constexpr void z(T z) noexcept requires (N >= 3) { _set_ind(2, z); }
      constexpr void w(T w) noexcept requires (N >= 4) { _set_ind(3, w); }

      // getters
      [[nodiscard]] constexpr T x() const noexcept requires (N >= 1) { return RowT::_data[0]; }
      [[nodiscard]] constexpr T y() const noexcept requires (N >= 2) { return RowT::_data[1]; }
      [[nodiscard]] constexpr T z() const noexcept requires (N >= 3) { return RowT::_data[2]; }
      [[nodiscard]] constexpr T w() const noexcept requires (N >= 4) { return RowT::_data[3]; }

      // cross product
      constexpr Vec cross(const Vec &other) const noexcept requires (N == 3) {
        std::array<T, 3> arr {
          RowT::_data[1] * other._data[2] - RowT::_data[2] * other._data[1],
          RowT::_data[2] * other._data[0] - RowT::_data[0] * other._data[2],
          RowT::_data[0] * other._data[1] - RowT::_data[1] * other._data[0]};

        stdx::fixed_size_simd<T, 3> ans;
        ans.copy_from(arr.data(), stdx::element_aligned);
        return {ans};
      }

      constexpr Vec operator%(const Vec &other) const noexcept requires (N == 3) {
        return cross(other);
      }

      // length methods
      [[nodiscard]] constexpr T length2() const noexcept {
        return stdx::reduce(RowT::_data * RowT::_data); // sum by default
      }

      [[nodiscard]] constexpr T length() const noexcept {
        return std::sqrt(length2());
      }

      [[nodiscard]] constexpr T inversed_length() const {
        return finv_sqrt(length2());
      }

      // use normalize_fast for higher precision
      constexpr Vec & normalize() noexcept {
        auto len = length2();
        if (std::abs(len) <= _epsilon) [[unlikely]] return *this;
        *this *= finv_sqrt(len);
        return *this;
      };

      // use normalized_fast for lower precision
      constexpr Vec normalized() const noexcept {
        auto len = length2();
        if (std::abs(len) <= _epsilon) [[unlikely]] return {};
        return Vec{*this * finv_sqrt(len)};
      };

      // use normalize for higher precision
      constexpr Vec & normalize_fast() noexcept {
        auto len = length2();
        if (std::abs(len) <= _epsilon) [[unlikely]] return *this;
        *this *= ffinv_sqrt(len);
        return *this;
      };

      // use normalized for higher precision
      constexpr Vec normalized_fast() const noexcept {
        auto len = length2();
        if (std::abs(len) <= _epsilon) [[unlikely]] return {};
        return Vec{*this * ffinv_sqrt(len)};
      };

      constexpr Vec & normalize_fast_unsafe() {
        auto l = length2();
        *this *= ffinv_sqrt(l);
        return *this;
      };

      constexpr Vec normalized_fast_unsafe() const {
        auto l = length2();
        return Vec{*this * ffinv_sqrt(l)};
      };

      // dot product
      [[nodiscard]] constexpr T dot(const Vec<T, N> other) const noexcept {
        return stdx::reduce(RowT::_data * other._data);
      }

      [[nodiscard]] constexpr T operator&(const Vec<T, N> other) const noexcept {
        return dot(other);
      }

      template<ArithmeticT R>
        constexpr Vec operator*(const Matr<R, N> &other) const noexcept {
          mr::Vec<T, N> tmp {};
          for (size_t i = 0; i < N; i++) {
            tmp._data += (other._data[i] * RowT::_data[i])._data;
          }
          return tmp;
        }

      template<ArithmeticT R>
        constexpr Vec operator*(const Matr<R, N + 1> &other) noexcept {
          mr::Vec<T, N + 1> copy = mr::Vec<T, N + 1>(*this);
          mr::Vec<T, N + 1> tmp {};

          for (size_t i = 0; i < N; i++) {
            tmp._data += (other._data[i] * copy._data[i])._data;
          }
          tmp._data += other._data[N]._data;
          return {tmp};
        }

      template<ArithmeticT R>
        constexpr Vec & operator*=(const Matr<R, N> &other) noexcept {
          mr::Vec<T, N> tmp {};
          for (size_t i = 0; i < N; i++) {
            tmp._data += (other._data[i] * RowT::_data[i])._data;
          }
          *this = tmp;
          return *this;
        }

      template<ArithmeticT R>
        constexpr Vec & operator*=(const Matr<R, N + 1> &other) noexcept {
          mr::Vec<T, N + 1> copy = mr::Vec<T, N + 1>(*this);
          mr::Vec<T, N + 1> tmp {};
          for (size_t i = 0; i < N; i++) {
            tmp._data += (other._data[i] * copy._data[i])._data;
          }
          tmp._data += other._data[N]._data;

          *this = mr::Vec<T, N>(tmp);
          return *this;
        }

      private:
        static constexpr T _epsilon = std::numeric_limits<T>::epsilon();
    };
} // namespace mr

#endif // __Vec_hpp_
