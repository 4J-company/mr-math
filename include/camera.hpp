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
          friend class Camera;
        public:
          T distance = 0.1f;
          T far = (1 << 10);
          T size = 0.1;

          T height = size;
          T width = size;

        private:
          // cached frustum matrix
          mutable bool frustum_calculated = false;
          mutable Matr4<T> frustum;

          // cached ortholinear matrix
          mutable bool ortholinear_calculated = false;
          mutable Matr4<T> ortholinear;
        };

        constexpr Camera() = default;
        constexpr Camera(Vec3<T> position) : _position(position) {}
        constexpr Camera(Vec3<T> position, Vec3<T> direction, Vec3<T> up = {0, 1, 0}) :
          _position(position), _direction(direction), _up(up) {
            _right = (_direction % _up).normalized();
          }

        // copy semantics
        constexpr Camera(const Camera &other) noexcept = default;
        constexpr Camera & operator=(const Camera &other) noexcept = default;

        // move semantics
        constexpr Camera(Camera &&other) noexcept = default;
        constexpr Camera & operator=(Camera &&other) noexcept = default;

        // position delta
        constexpr Camera & operator+=(Vec3<T> position_delta) noexcept {
          _perspective_calculated = false;
          _position += position_delta;
        }

        // angle in radians
        constexpr Camera & operator+=(Pitch angle_rad) noexcept {
          _perspective_calculated = false;
          _direction *= Matr4<T>::rotate(angle_rad, _right);
          _up *= Matr4<T>::rotate(angle_rad, _right);
        }

        // angle in radians
        constexpr Camera & operator+=(Yaw angle_rad) noexcept {
          _perspective_calculated = false;
          _direction *= Matr4<T>::rotate(angle_rad, _up);
          _right *= Matr4<T>::rotate(angle_rad, _up);
        }

        // angle in radians
        constexpr Camera & operator+=(Roll angle_rad) noexcept {
          _perspective_calculated = false;
          _right *= Matr4<T>::rotate(angle_rad, _direction);
          _up *= Matr4<T>::rotate(angle_rad, _direction);
        }
        
        constexpr Vec3<T> position() const noexcept {
          return _position;
        }

        constexpr void position(Vec3<T> pos) noexcept {
          _perspective_calculated = false;
          _position = pos;
        }

        constexpr Vec3<T> direction() const noexcept {
          return _direction;
        }

        constexpr void direction(Vec3<T> dir) noexcept {
          _perspective_calculated = false;
          _direction = dir;
        }

        constexpr Projection & projection() noexcept {
          _perspective_calculated = false;
          return _projection;
        }

        constexpr const Projection & projection() const noexcept {
          return _projection;
        }

        constexpr Matr4<T> perspective() const noexcept {
          if (_perspective_calculated) {
            return _perspective;
          }

          _perspective_calculated = true;
          _perspective = mr::Matr4<T>{
            typename mr::Matr4<T>::Row_t{_right[0], _up[0], _direction[0], 0},
            typename mr::Matr4<T>::Row_t{_right[1], _up[1], _direction[1], 0},
            typename mr::Matr4<T>::Row_t{_right[2], _up[2], _direction[2], 0},
            typename mr::Matr4<T>::Row_t{-(_position & _right), -(_position & _up), (_position & _direction), 1}
          };
          return _perspective;
        }

        constexpr Matr4<T> ortholinear() const noexcept {
          if (_projection.ortholinear_calculated) {
            return _projection.ortholinear;
          }

          const T l = -_projection.height / 2; // left
          const T r = _projection.height / 2;  // right
          const T b = -_projection.width / 2;  // bottom
          const T t = _projection.width / 2;   // top
          const T n = _projection.distance;    // near
          const T f = _projection.far;         // far

          _projection.ortholinear_calculated = true;
          _projection.ortholinear = mr::Matr4<T>{
            typename mr::Matr4<T>::Row_t(2 / (r - l), 0, 0, 0),
            typename mr::Matr4<T>::Row_t(0, 2 / (t - b), 0, 0),
            typename mr::Matr4<T>::Row_t(0, 0, 2 / (n - f), 0),
            typename mr::Matr4<T>::Row_t((r + l) / (l - r), (t + b) / (b - t), (f + n) / (n - f), 1)
          };
          return _projection.ortholinear;
        }

        constexpr Matr4<T> frustum() const noexcept {
          if (_projection.frustum_calculated) {
            return _projection.frustum;
          }

          const T l = -_projection.height / 2; // left
          const T r = _projection.height / 2;  // right
          const T b = -_projection.width / 2;  // bottom
          const T t = _projection.width / 2;   // top
          const T n = _projection.distance;    // near
          const T f = _projection.far;         // far

          _projection.frustum_calculated = true;
          _projection.frustum = mr::Matr4<T>{
            typename mr::Matr4<T>::Row_t(2 * n / (r - l), 0, 0, 0),
            typename mr::Matr4<T>::Row_t(0, 2 * n / (t - b), 0, 0),
            typename mr::Matr4<T>::Row_t((r + l) / (r - l), (t + b) / (t - b), (f + n) / (n - f), -1),
            typename mr::Matr4<T>::Row_t(0, 0, 2 * n * f / (n - f), 0)
          };
          return _projection.frustum;
        }

      private:
        Vec3<T> _position;
        Vec3<T> _direction;
        Vec3<T> _up;
        Vec3<T> _right;
        Projection _projection;

        mutable bool _perspective_calculated = false;
        mutable Matr4<T> _perspective;
    };
}

#endif // __camera_hpp_
