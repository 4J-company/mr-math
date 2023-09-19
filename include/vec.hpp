#ifndef __Vec_hpp_
#define __Vec_hpp_

#include "def.hpp"
#include <pstl/glue_execution_defs.h>

namespace mr {
  template<typename T, std::size_t N>
    class Vec;

  template<typename T>
    using Vec2 = Vec<T, 2>;
  template<typename T>
    using Vec3 = Vec<T, 3>;
  template<typename T>
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

  template<typename T, std::size_t N>
    class Vec {
      public:
        constexpr inline static std::size_t size = N;

      private:
        stdx::fixed_size_simd<T, N> _data;

      public:
        // default constructor
        Vec() = default;

        // from simd constructor
        Vec(stdx::fixed_size_simd<T, N> data) : _data(data) {}

        // variable number of arguments constructor
        template<typename... Args>
          Vec(Args... args) {
            static_assert(1 <= sizeof...(args) && sizeof...(args) <= N, "Wrong number of parameters");
            std::array<T, N> arr {args...};
            _data.copy_from(&arr[0], stdx::element_aligned);
          }

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

        [[nodiscard]] constexpr Vec operator+(const Vec &other) const noexcept {
          return _data + other._data;
        }

        [[nodiscard]] constexpr Vec operator-(const Vec &other) const noexcept {
          return _data - other._data;
        }

        [[nodiscard]] constexpr Vec operator*(const Vec &other) const noexcept {
          return _data * other._data;
        }

        [[nodiscard]] constexpr Vec operator/(const Vec &other) const noexcept {
          return _data / other._data;
        }

        [[nodiscard]] constexpr T length2() const noexcept {
          return stdx::reduce(_data * _data); // sum by default
        }

        [[nodiscard]] constexpr T length() const noexcept {
          return std::sqrt(length2());
        }

        [[nodiscard]] constexpr T operator[](std::size_t i) const {
          return _data[i];
        }

        template<typename... Args>
          constexpr void shuffle(Args... args) {
            static_assert(1 <= sizeof...(args) && sizeof...(args) <= N, "Wrong number of parameters");
            std::array<int, N> tmp {static_cast<int>(args)...};
            std::array<T, N> arr;
            std::for_each(std::execution::unseq, tmp.begin(), tmp.end(), [&](auto i) {
                arr[i] = _data[i];
                });
            _data.copy_from(&arr[0], stdx::element_aligned);
          }

        template<typename... Args>
          [[nodiscard]] constexpr Vec shuffled(Args... args) const noexcept {
            static_assert(1 <= sizeof...(args) && sizeof...(args) <= N, "Wrong number of parameters");

            std::array<int, N> arr {};
            std::ranges::for_each(
                std::execution::par_unseq, 
                std::views::zip(
                  std::views::iota(0, static_cast<int>(size)),
                  std::array<T, N>(static_cast<int>(args)...)), 
                [&](auto e) {
                arr[std::get<0>(e)] = _data[std::get<1>(e)];
                });

            stdx::fixed_size_simd<T, N> ans;
            ans.copy_from(&arr[0], stdx::element_aligned);

            return ans;
          }

        [[nodiscard]] constexpr Vec & normalize() noexcept {
          _data /= length();
          return *this;
        };

        [[nodiscard]] constexpr Vec normalized() const noexcept {
          return {_data / length()};
        };

        [[nodiscard]] constexpr Vec & normalize_safe() noexcept {
          auto l = length();
          if (l == 0) return *this;
          _data /= l;
          return *this;
        };

        [[nodiscard]] constexpr Vec normalized_safe() const noexcept {
          auto l = length();
          if (l == 0) return {};
          return {_data / l};
        };

        // cross product
        [[nodiscard]] constexpr Vec operator%(const Vec<T, N> other) const noexcept {
          Vec<T, N> tmp0 = this->shuffled(3, 0, 2, 1);
          Vec<T, N> tmp1 = other.shuffled(3, 1, 0, 2);
          Vec<T, N> tmp2 = tmp0 * other._data;
          Vec<T, N> tmp3 = tmp0 * tmp1;
          Vec<T, N> tmp4 = tmp2.shuffled(3, 0, 2, 1);

          return tmp3 - tmp4;
        }

        // dot product
        [[nodiscard]] constexpr T operator^(const Vec<T, N> other) const noexcept {
        }

        constexpr bool operator<=>(const Vec &other) const noexcept = delete;

        friend std::ostream & operator<<(std::ostream &s, const Vec &v) noexcept {
          s << '(';
          for (int i = 0; i < N; i++)
            s << v[i] 
              << (char)(',' * (i < N - 1)) 
              << (char)(' ' * (i < N - 1));
          s << ')';
          return s;
        }
    };
}

#endif // __Vec_hpp_
