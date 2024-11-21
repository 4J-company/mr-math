#ifndef __bound_box_hpp_
#define __bound_box_hpp_

#include "def.hpp"
#include "vec.hpp"

namespace mr {
  template <ArithmeticT T>
    class AABB;

  using AABBf = AABB<float>;
  using AABBd = AABB<double>;
  using AABBi = AABB<int>;
  using AABBu = AABB<uint32_t>;

  template <ArithmeticT T>
    class AABB {
    public:
      using VecT = Vec3<T>;

      constexpr VecT dimensions() const noexcept { return max - min; }

      constexpr bool contains(const VecT &point) const noexcept {
        return
          min.x() <= point.x() && point.x() <= max.x() &&
          min.y() <= point.y() && point.y() <= max.x() &&
          min.z() <= point.z() && point.z() <= max.z();
      }

      constexpr bool contains(const AABB &other) const noexcept {
        return
          min.x() <= other.min.x() && other.max.x() <= max.x() &&
          min.y() <= other.min.y() && other.max.y() <= max.x() &&
          min.z() <= other.min.z() && other.max.z() <= max.z();
      }

      constexpr bool intersects(const AABB &other) const noexcept {
        return
          min.x() <= other.max.x() && other.min.x() <= max.x() &&
          min.y() <= other.max.y() && other.min.y() <= max.x() &&
          min.z() <= other.max.z() && other.min.z() <= max.z();
      }

      VecT min;
      VecT max;
    };
}

#endif // __bound_box_hpp_