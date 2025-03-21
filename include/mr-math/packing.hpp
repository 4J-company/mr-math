#ifndef __MR_PACKING_HPP_
#define __MR_PACKING_HPP_

#include "norm.hpp"
#include "vec.hpp"

namespace mr {
inline namespace math {
namespace details {

  Vec2f sign_non_zero(const Vec2f& v)
  {
    return Vec2f{v.x() >= 0, v.y() >= 0} * 2 - Vec2f{1};
  }

  Vec2f oct_wrap(const Vec2f& v)
  {
    return (Vec2f{1} - std::abs(Vec2f{v.y(), v.x()})) * sign_non_zero(v);
  }

} // namespace details

  // octahedron normal packing
  using PackedNormOct = Vec2f;

  PackedNormOct pack_oct(const Norm3f& normal)
  {
    const Vec2f oct_proj = Vec2f{normal.x(), normal.y()} / (std::abs(normal.x()) + std::abs(normal.y()) + std::abs(normal.z()));
    return normal.z() > 0 ? oct_proj : mr::details::oct_wrap(oct_proj);
  }

  Norm3f unpack_oct(const PackedNormOct& normal) {
    Vec3f vec{normal.x(), normal.y(), 1 - std::abs(normal.x()) - std::abs(normal.y())};
    if (vec.z() < 0) {
      vec = Vec3f{(Vec2f{1} - std::abs(Vec2f{vec.y(), vec.x()})) * mr::details::sign_non_zero(Vec2f{vec.x(), vec.y()}), vec.z()};
    }
    return vec.normalized_unchecked();
  }



} // namespace math
} // namespace mr

#endif // __MR_PACKING_HPP_
