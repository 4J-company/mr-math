#ifndef __operators_matr_hpp_
#define __operators_matr_hpp_

#include "../def.hpp"
#include "mr-math/vec.hpp"

namespace mr {
  template <typename DerivedT>
    struct MatrOperators {
      using RowT = DerivedT::RowT;
      inline static constexpr size_t N = DerivedT::size;

      // basic math operations
      friend constexpr DerivedT &
      operator*=(DerivedT &self, const DerivedT &other) noexcept {
        std::array<RowT, N> tmp {};
        for (size_t i = 0; i < N; i++) {
          for (size_t j = 0; j < N; j++) {
            tmp[j] += other._data[i] * self._data[j][i];
          }
        }
        self = tmp;
        return self;
      }

      friend constexpr DerivedT &
      operator+=(DerivedT &self, const DerivedT &other) noexcept {
        for (int i = 0; i < N; i++) {
          self._data[i] += other._data[i];
        }
        return self;
      }

      friend constexpr DerivedT &
      operator-=(DerivedT &self, const DerivedT &other) noexcept {
        for (int i = 0; i < N; i++) {
          self._data[i] -= other._data[i];
        }
        return self;
      }

      friend constexpr DerivedT
      operator*(const DerivedT &self, const DerivedT &other) noexcept {
        std::array<RowT, N> tmp{};
        for (size_t i = 0; i < N; i++) {
          for (size_t j = 0; j < N; j++) {
            tmp[j] += other._data[i] * self._data[j][i];
          }
        }
        return {tmp};
      }

      friend constexpr DerivedT
      operator+(const DerivedT &self, const DerivedT &other) noexcept {
        std::array<RowT, N> tmp;
        for (size_t i = 0; i < N; i++) {
          tmp[i] = static_cast<RowT>(self._data[i] + other._data[i]);
        }
        return {tmp};
      }

      friend constexpr DerivedT
      operator-(const DerivedT &self, const DerivedT &other) noexcept {
        std::array<RowT, N> tmp;
        for (size_t i = 0; i < N; i++) {
          tmp[i] = self._data[i] - other._data[i];
        }
        return {tmp};
      }

      friend std::ostream & operator<<(std::ostream &s, const DerivedT &m) noexcept {
        std::cout << std::endl;
        for (size_t i = 0; i < N; i++) {
          std::cout << m._data[i] << std::endl;
        }
        return s;
      }

    private:
      static DerivedT _identity() {
        std::array<typename DerivedT::RowT, N> id;
        constexpr auto io = std::ranges::iota_view {(size_t)0, N};

        std::ranges::transform(io, id.begin(),
          [&io](auto i) -> RowT {
            return SimdImpl<typename DerivedT::ValueT, DerivedT::size>(
                [i](auto i2) { return i2 == i ? 1 : 0; });
          });

        return id;
      }
    };

  template <typename Scale>
    struct ScaleMatrOperators {
      template <typename T>
        friend Vec3<T> operator*(const Vec3<T> &vec, const Scale &scale) {
          return {vec * scale.scale_vector};
        }
      template <typename T>
        friend Vec3<T> operator*=(Vec3<T> &vec, const Scale &scale) {
          return vec *= scale.scale_vector;
        }

      template <typename T>
        friend Vec4<T> operator*(const Vec4<T> &vec, const Scale &scale) {
          return {vec * scale.scale_vector};
        }
      template <typename T>
        friend Vec4<T> operator*=(Vec4<T> &vec, const Scale &scale) {
          return vec *= scale.scale_vector;
        }

      friend Scale operator*(const Scale &self, const Scale &other) {
        return {self.scale_vector * other.scale_vector};
      }

      friend Scale operator*=(Scale &self, const Scale &other) {
        self.scale_vector *= other.scale_vector;
        return self;
      }

      friend Scale operator+(const Scale &self, const Scale &other) {
        return {self.scale_vector + other.scale_vector};
      }

      friend Scale operator+=(Scale &self, const Scale &other) {
        self.scale_vector += other.scale_vector;
        return self;
      }

      friend Scale operator-(const Scale &self, const Scale &other) {
        return {self.scale_vector - other.scale_vector};
      }

      friend Scale operator-=(Scale &self, const Scale &other) {
        self.scale_vector -= other.scale_vector;
        return self;
      }
    };

  template <typename Translate>
    struct TranslateMatrOperators {
      template <typename T>
        friend Vec3<T> operator*(const Vec3<T> &vec, const Translate &scale) {
          return {vec * scale.translate_vector};
        }
      template <typename T>
        friend Vec3<T> operator*=(Vec3<T> &vec, const Translate &scale) {
          return vec *= scale.translate_vector;
        }

      template <typename T>
        friend Vec4<T> operator*(const Vec4<T> &vec, const Translate &scale) {
          return {vec * scale.translate_vector};
        }
      template <typename T>
        friend Vec4<T> operator*=(Vec4<T> &vec, const Translate &scale) {
          return vec *= scale.translate_vector;
        }

      friend Translate operator*(const Translate &self, const Translate &other) {
        return {self.translate_vector + other.translate_vector};
      }

      friend Translate operator*=(Translate &self, const Translate &other) {
        self.translate_vector += other.translate_vector;
        return self;
      }

      friend Translate operator+(const Translate &self, const Translate &other) {
        return {self.translate_vector + other.translate_vector};
      }

      friend Translate operator+=(Translate &self, const Translate &other) {
        self.translate_vector += other.translate_vector;
        return self;
      }

      friend Translate operator-(const Translate &self, const Translate &other) {
        return {self.translate_vector - other.translate_vector};
      }

      friend Translate operator-=(Translate &self, const Translate &other) {
        self.translate_vector -= other.translate_vector;
        return self;
      }
    };
}

#endif // __operators_matr_hpp_
