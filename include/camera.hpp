#ifndef __camera_hpp_
#define __camera_hpp_

#include "def.hpp"
#include "vec.hpp"
#include "matr.hpp"

namespace mr {

  template <typename T = float> requires std::is_floating_point_v<T>
    class [[nodiscard]] Camera {
      public:
        struct Yaw : T {};
        struct Pitch : T {};
        struct Roll : T {};

        struct Projection {
          T distance = 0.1f, far = (1 << 10), size = 0.1;
        };

      private:
        inline static constexpr std::size_t N = 3;

        Vec3<T> _position;
        Vec3<T> _direction;
        Vec3<T> _up;
        Vec3<T> _right;
        Projection _projection;

      public:
        constexpr Camera() = default;
        constexpr Camera(Vec3<T> position) : _position(position) {}
        constexpr Camera(Vec3<T> position, Vec3<T> direction, Vec3<T> up = {0, 1, 0}) :
          _position(position), _direction(direction), _up(up) {
            _right = (_direction % _up).normalized();
          }

        // copy semantics
        Camera(const Camera &other) noexcept = default;
        Camera & operator=(const Camera &other) noexcept = default;

        // move semantics
        Camera(Camera &&other) noexcept = default;
        Camera & operator=(Camera &&other) noexcept = default;

        // angle in radians
        constexpr Camera & operator+=(Pitch angle_rad) noexcept {
          _direction *= Matr4<T>::rotate(angle_rad, _right);
          _up *= Matr4<T>::rotate(angle_rad, _right);
        }

        // angle in radians
        constexpr Camera & operator+=(Yaw angle_rad) noexcept {
          _direction *= Matr4<T>::rotate(angle_rad, _up);
          _right *= Matr4<T>::rotate(angle_rad, _up);
        }

        // angle in radians
        constexpr Camera & operator+=(Roll angle_rad) noexcept {
          _right *= Matr4<T>::rotate(angle_rad, _direction);
          _up *= Matr4<T>::rotate(angle_rad, _direction);
        }

        constexpr Matr4<T> perspective() const noexcept {
          return {
            mr::Matr4<T>::Row_t(_right[0], _up[0], _direction[0], 0),
            mr::Matr4<T>::Row_t(_right[1], _up[1], _direction[1], 0),
            mr::Matr4<T>::Row_t(_right[2], _up[2], _direction[2], 0),
            mr::Matr4<T>::Row_t(-(_position & _right), -(_position & _up), (_position & _direction), 1)
          };
        }

        constexpr Matr4<T> ortholinear() const noexcept {
          return {};
        }

        constexpr Matr4<T> frustum() const noexcept {
          return {};
        }
    };
}

#endif // __camera_hpp_
