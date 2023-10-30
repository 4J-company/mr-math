#ifndef __Vec_hpp_
#define __Vec_hpp_

#include "def.hpp"
#include "row.hpp"

namespace mr {
  template <typename T, std::size_t N>
    struct Vec;

  template <typename T>
    using Vec2 = Vec<T, 2>;
  template <typename T>
    using Vec3 = Vec<T, 3>;
  template <typename T>
    using Vec4 = Vec<T, 4>;

  using Vec2i = Vec<int, 2>;
  using Vec3i = Vec<int, 3>;
  using Vec4i = Vec<int, 4>;

  using Vec2u = Vec<int, 2>;
  using Vec3u = Vec<int, 3>;
  using Vec4u = Vec<int, 4>;

  using Vec2f = Vec<float, 2>;
  using Vec3f = Vec<float, 3>;
  using Vec4f = Vec<float, 4>;

  using Vec2d = Vec<float, 2>;
  using Vec3d = Vec<float, 3>;
  using Vec4d = Vec<float, 4>;

  template <typename T, std::size_t N>
    struct [[nodiscard]] Vec : Row<T, N> {
        // default constructor
        constexpr Vec() = default;

        template <
          typename ... Args
        > requires ((sizeof...(Args) == 0) || (sizeof...(Args) == N)) &&
                   (std::is_convertible_v<Args, T> && ...)
        constexpr Vec(const Args ...args) : Row<T, N>(args...) {}

        // from simd constructor
        constexpr Vec(Row<T, N> data) : Row<T, N>(data) {}

        // move semantics
        constexpr Vec(Vec &&) noexcept = default;
        constexpr Vec &operator=(Vec &&) noexcept = default;

        // copy semantics
        constexpr Vec(const Vec &other) noexcept = default;
        constexpr Vec & operator=(const Vec &other) noexcept = default;

        [[nodiscard]] constexpr T length2() const noexcept {
          return stdx::reduce(this->_data * this->_data); // sum by default
        }

        [[nodiscard]] constexpr T length() const noexcept {
          return std::sqrt(length2());
        }

        [[nodiscard]] constexpr T inversed_length() const {
          return finv_sqrt(length2());
        }

        constexpr Vec & normalize() noexcept {
          auto l = length2();
          constexpr double E = 0.001;
          if (std::abs(l - E) < 0) return *this;
          *this *= finv_sqrt(l);
          return *this;
        };

        constexpr Vec normalized() const noexcept {
          auto l = length2();
          constexpr double E = 0.001;
          if (std::abs(l - E) < 0) return {};
          return {*this * finv_sqrt(l)};
        };

        constexpr Vec & normalize_fast() noexcept {
          auto l = length2();
          constexpr double E = 0.001;
          if (std::abs(l - E) < 0) return *this;
          *this *= ffinv_sqrt(l);
          return *this;
        };

        constexpr Vec normalized_fast() const noexcept {
          auto l = length2();
          constexpr double E = 0.001;
          if (std::abs(l - E) < 0) return {};
          return {*this * ffinv_sqrt(l)};
        };

        // cross product
        constexpr Vec operator%(const Vec<T, N> &other) const noexcept {
          static_assert(N == 3, "Wrong number of elements");

          std::array<T, 3> arr {
            this->_data[1] * other._data[2] - this->_data[2] * other._data[1],
            this->_data[2] * other._data[0] - this->_data[0] * other._data[2],
            this->_data[0] * other._data[1] - this->_data[1] * other._data[0]
          };
          stdx::fixed_size_simd<T, N> ans;
          ans.copy_from(arr.data(), stdx::element_aligned);
          return {ans};
        }

        // dot product
        [[nodiscard]] constexpr T operator&(const Vec<T, N> other) const noexcept {
          return stdx::reduce(*this * other);
        }

        // friend std::ostream & operator<<(std::ostream &s, const Vec &v) noexcept {
        //   s << v._data;
        //   return s;
        // }
    };
}

#endif // __Vec_hpp_
