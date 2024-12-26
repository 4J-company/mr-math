#ifndef __MR_CAMERA_HPP_
#define __MR_CAMERA_HPP_

#include "vec.hpp"
#include "rot.hpp"
#include "matr.hpp"

namespace mr {
  using namespace mr::literals;

  template <std::floating_point T = float>
    struct [[nodiscard]] Camera {
      public:
        using ValueT = T;
        using VecT = Vec3<T>;
        using NormT = Norm3<T>;
        using MatrT = Matr4<T>;

        struct Projection {
          friend struct Camera;
        public:
          T distance = 0.001f;
          T far = (1 << 10);

          T width = 0.16;
          T height = 0.09;

          inline constexpr Projection(mr::Radiansf fov = mr::Radiansf(90_deg),
                                      float aspect_ratio = 16.f / 9.f) noexcept
              : width(2 * distance * std::tan(fov._data / 2))
              , height(width * aspect_ratio) { }

          inline constexpr Projection(Projection &&) noexcept = default;
          inline constexpr Projection & operator=(Projection &&) noexcept = default;
          inline constexpr Projection(const Projection &) noexcept = default;
          inline constexpr Projection & operator=(const Projection &) noexcept = default;

          inline constexpr void resize(float aspect_ratio) {
            height = width * aspect_ratio;
          }

        private:
          // cached frustum matrix
          mutable bool frustum_calculated = false;
          mutable MatrT frustum;

          // cached orthographics matrix
          mutable bool orthographic_calculated = false;
          mutable MatrT orthographic;
        };

        constexpr Camera() = default;
        constexpr Camera(VecT position) : _position(position) {}
        constexpr Camera(VecT position, VecT direction, VecT up = {0, 1, 0}) :
          _position(position),
          _rotation(
              direction.normalized(),
              up.normalized(),
              (direction % up).normalized()) {}

        // copy semantics
        constexpr Camera(const Camera &other) noexcept {
          _position   = other._position;
          _rotation   = other._rotation;
          _projection = other._projection;

          _perspective_calculated = false;
        }

        constexpr Camera &operator=(const Camera &other) noexcept {
          _position   = other._position;
          _rotation   = other._rotation;
          _projection = other._projection;

          _perspective_calculated = false;
          return *this;
        }

        // move semantics
        constexpr Camera(Camera &&other) noexcept {
          _position   = std::move(other._position);
          _rotation   = std::move(other._rotation);
          _projection = std::move(other._projection);

          _perspective_calculated = false;
        }

        constexpr Camera &operator=(Camera &&other) noexcept {
          _position   = std::move(other._position);
          _rotation   = std::move(other._rotation);
          _projection = std::move(other._projection);

          _perspective_calculated = false;
          return *this;
        }

        // position delta
        constexpr Camera & operator+=(VecT position_delta) noexcept {
          _perspective_calculated = false;
          _position += position_delta;

          return *this;
        }

        // angle in radians
        constexpr Camera & operator+=(Pitch<T> angle_rad) noexcept {
          _perspective_calculated = false;
          _rotation += angle_rad;

          return *this;
        }

        // angle in radians
        constexpr Camera & operator+=(Yaw<T> angle_rad) noexcept {
          _perspective_calculated = false;
          _rotation += angle_rad;

          return *this;
        }

        // angle in radians
        constexpr Camera & operator+=(Roll<T> angle_rad) noexcept {
          _perspective_calculated = false;
          _rotation += angle_rad;

          return *this;
        }

        constexpr VecT position() const noexcept {
          return _position;
        }

        constexpr void position(VecT pos) noexcept {
          _perspective_calculated = false;
          _position = pos;
        }

        constexpr NormT direction() const noexcept {
          return _rotation.direction();
        }

        constexpr void direction(NormT dir) noexcept {
          _perspective_calculated = false;
          _rotation.direction(dir);
        }

        constexpr NormT right() const noexcept {
          return _rotation.right();
        }

        constexpr NormT up() const noexcept {
          return _rotation.up();
        }

        constexpr Projection & projection() noexcept {
          _perspective_calculated = false;
          return _projection;
        }

        constexpr const Projection & projection() const noexcept {
          return _projection;
        }

        constexpr MatrT perspective() const noexcept {
          if (_perspective_calculated) [[likely]] {
            return _perspective;
          }
          return calculate_perspective();
        }

        constexpr MatrT orthographic() const noexcept {
          if (_projection.orthographic_calculated) [[likely]] {
            return _projection.orthographic;
          }
          return calculate_orthographic();
        }

        constexpr MatrT frustum() const noexcept {
          if (_projection.frustum_calculated) [[likely]] {
            return _projection.frustum;
          }
          return calculate_frustum();
        }

        constexpr MatrT calculate_perspective() const noexcept {
          std::lock_guard lg(_perspective_mutex);

          const auto direction = -_rotation.direction();
          const auto right = _rotation.right();
          const auto up = _rotation.up();
          _perspective = MatrT{
                       right.x(),            up.x(),           direction.x(), 0,
                       right.y(),            up.y(),           direction.y(), 0,
                       right.z(),            up.z(),           direction.z(), 0,
            -(_position & right), -(_position & up), -(_position & direction), 1
          };
          _perspective_calculated = true;
          return _perspective;
        }

        constexpr MatrT calculate_orthographic() const noexcept {
          std::lock_guard lg(_perspective_mutex);

          const T l = -_projection.height / 2; // left
          const T r = _projection.height / 2;  // right
          const T b = -_projection.width / 2;  // bottom
          const T t = _projection.width / 2;   // top
          const T n = _projection.distance;    // near
          const T f = _projection.far;         // far

          _projection.orthographic = MatrT {
                  2 / (r - l),                 0,                 0, 0,
                            0,       2 / (t - b),                 0, 0,
                            0,                 0,       2 / (n - f), 0,
            (r + l) / (l - r), (t + b) / (b - t), (f + n) / (n - f), 1
          };
          _projection.orthographic_calculated = true;
          return _projection.orthographic;
        }

        constexpr MatrT calculate_frustum() const noexcept {
          std::lock_guard lg(_perspective_mutex);

          const T l = -_projection.height / 2; // left
          const T r = _projection.height / 2;  // right
          const T b = -_projection.width / 2;  // bottom
          const T t = _projection.width / 2;   // top
          const T n = _projection.distance;    // near
          const T f = _projection.far;         // far

          _projection.frustum = MatrT{
              2 * n / (r - l),                 0,                   0,  0,
                            0,   2 * n / (t - b),                   0,  0,
            (r + l) / (r - l), (t + b) / (t - b),   (f + n) / (n - f), -1,
                            0,                 0, 2 * n * f / (n - f),  0
          };
          _projection.frustum_calculated = true;
          return _projection.frustum;
        }

      private:
        VecT _position;
        Rotation<T> _rotation;
        Projection _projection;

        mutable std::mutex _perspective_mutex;
        mutable std::atomic_bool _perspective_calculated = false;
        mutable MatrT _perspective;
    };
}

#endif // __MR_CAMERA_HPP_
