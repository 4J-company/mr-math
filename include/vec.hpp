#ifndef __Vec_hpp_
#define __Vec_hpp_

#include "def.hpp"
#include "row.hpp"

namespace mr {
  template <typename T, std::size_t N>
    class Vec;

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
    class [[nodiscard]] Vec {
      private:
        Row<T, N> _data;

      public:
        // default constructor
        constexpr Vec() = default;

        // from simd constructor
        constexpr Vec(Row<T, N> data) : _data(data) {}

        // move semantics
        Vec(Vec &&) noexcept = delete;
        Vec &operator=(Vec &&) noexcept = delete;

        // copy semantics
        Vec(const Vec &other) noexcept = default;
        Vec & operator=(const Vec &other) noexcept = default;

        constexpr Vec & operator+=(const Vec &other) noexcept {
          _data += other._data;
          return *this;
        }

        constexpr Vec & operator-=(const Vec &other) noexcept {
          _data -= other._data;
          return *this;
        }

        constexpr Vec & operator*=(const Vec &other) noexcept {
          _data *= other._data;
          return *this;
        }

        constexpr Vec & operator/=(const Vec &other) noexcept {
          _data /= other._data;
          return *this;
        }

        constexpr Vec operator+(const Vec &other) const noexcept {
          return _data + other._data;
        }

        constexpr Vec operator-(const Vec &other) const noexcept {
          return _data - other._data;
        }

        constexpr Vec operator*(const Vec &other) const noexcept {
          return _data * other._data;
        }

        constexpr Vec operator/(const Vec &other) const noexcept {
          return _data / other._data;
        }

        [[nodiscard]] constexpr T length2() const noexcept {
          return stdx::reduce(_data * _data); // sum by default
        }

        [[nodiscard]] constexpr T length() const noexcept {
          return std::sqrt(length2());
        }

        [[nodiscard]] constexpr T inversed_length() const {
          float number = length2();
          long i;
          float x2, y;
          const float threehalfs = 1.5F;

          x2 = number * 0.5F;
          y  = number;
          i  = * ( long * ) &y;                        // evil floating point bit level hacking
          i  = 0x5f3759df - ( i >> 1 );                // what the fuck?
          y  = * ( float * ) &i;
          y  = y * ( threehalfs - ( x2 * y * y ) );    // 1st iteration
          // y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

          return y;
        }

        [[nodiscard]] constexpr T operator[](std::size_t i) const {
          return _data[i];
        }

        constexpr Vec & normalize() {
          _data *= inversed_length();
          return *this;
        };

        constexpr Vec normalized() const {
          return {_data * inversed_length()};
        };

        constexpr Vec & normalize_safe() noexcept {
          auto l = length();
          constexpr double E = 0.001;
          if (std::abs(l - E) == 0) return *this;
          _data /= l;
          return *this;
        };

        constexpr Vec normalized_safe() const noexcept {
          auto l = length();
          constexpr double E = 0.001;
          if (std::abs(l - E) == 0) return {};
          return {_data / l};
        };

        // cross product
        constexpr Vec operator%(const Vec<T, N> other) const noexcept {
          static_assert(N == 3, "Wrong number of elements");

          std::array<T, 3> arr {
            _data[1] * other._data[2] - _data[2] * other._data[1],
            _data[2] * other._data[0] - _data[0] * other._data[2],
            _data[0] * other._data[1] - _data[1] * other._data[0]
          };
          stdx::fixed_size_simd<T, N> ans;
          ans.copy_from(&arr[0], stdx::element_aligned);
          return {ans};

          // Vec tmp0 = this->shuffled(1, 2, 0);
          // Vec tmp1 = other.shuffled(2, 0, 1);
          // Vec tmp2 = this->shuffled(2, 0, 1);
          // Vec tmp3 = other.shuffled(1, 2, 0);

          // return tmp0 * tmp1 - tmp2 * tmp3;
        }

        // dot product
        [[nodiscard]] constexpr T operator&(const Vec<T, N> other) const noexcept {
          return stdx::reduce(_data * other._data);
        }

        friend std::ostream & operator<<(std::ostream &s, const Vec &v) noexcept {
          s << v._data;
          return s;
        }
    };
}

#endif // __Vec_hpp_
