#ifndef __MR_CAMERA_HPP_
#define __MR_CAMERA_HPP_

#include "vec.hpp"
#include "matr.hpp"

namespace mr {
inline namespace math {

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

        constexpr Camera(VecT position = {0, 0, 0}, VecT direction = mr::axis::z, VecT up = mr::axis::y)
          : _position(position)
          , _direction(direction.normalized_unchecked())
          , _right(direction.cross(up).normalized_unchecked())
          , _up(up.normalized_unchecked())
          {}

        // copy semantics
        constexpr Camera(const Camera &other) noexcept
          : _position   {other._position}
          , _direction  {other._direction}
          , _up         {other._up}
          , _right      {other._right}
          , _projection {other._projection}
        {
          _perspective_calculated = false;
        }

        constexpr Camera &operator=(const Camera &other) noexcept {
          _position   = other._position;
          _direction  = other._direction;
          _up         = other._up;
          _right      = other._right;
          _projection = other._projection;

          _perspective_calculated = false;
          return *this;
        }

        // move semantics
        constexpr Camera(Camera &&other) noexcept
          : _position   {std::move(other._position)}
          , _direction  {std::move(other._direction)}
          , _up         {std::move(other._up)}
          , _right      {std::move(other._right)}
          , _projection{ std::move(other._projection) }
        {
          _perspective_calculated = false;
        }

        constexpr Camera &operator=(Camera &&other) noexcept {
          _position   = std::move(other._position);
          _direction  = std::move(other._direction);
          _up         = std::move(other._up);
          _right      = std::move(other._right);
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

          auto rot = mr::rotate(right(), angle_rad.value);
          _direction *= rot;
          _up *= rot;

          return *this;
        }

        // angle in radians
        constexpr Camera & operator+=(Yaw<T> angle_rad) noexcept {
          _perspective_calculated = false;

          auto rot = mr::rotate(up(), -angle_rad.value);
          _direction *= rot;
          _right *= rot;

          return *this;
        }

        // angle in radians
        constexpr Camera & operator+=(Roll<T> angle_rad) noexcept {
          _perspective_calculated = false;

          auto rot = mr::rotate(direction(), angle_rad.value);
          _up *= rot;
          _right *= rot;

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
          return _direction;
        }

        /*
        constexpr void direction(NormT dir) noexcept {
          _perspective_calculated = false;
          _rotation.direction(dir);
        }
        */

        constexpr NormT right() const noexcept {
          return _right;
        }

        constexpr NormT up() const noexcept {
          return _up;
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

          const auto direction = -_direction;
          const auto right = _right;
          const auto up = _up;
          _perspective = MatrT{
                          right.x(),               up.x(),               direction.x(), 0,
                          right.y(),               up.y(),               direction.y(), 0,
                          right.z(),               up.z(),               direction.z(), 0,
            -(_position.dot(right)), -(_position.dot(up)), -(_position.dot(direction)), 1
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
        Projection _projection;

        VecT _position;
        NormT _direction;
        NormT _up;
        NormT _right;

        mutable std::mutex _perspective_mutex;
        mutable std::atomic_bool _perspective_calculated = false;
        mutable MatrT _perspective;
    };
}
}

#endif // __MR_CAMERA_HPP_
