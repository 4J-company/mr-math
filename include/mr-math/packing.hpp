#ifndef __MR_PACKING_HPP_
#define __MR_PACKING_HPP_

#include "norm.hpp"
#include "vec.hpp"

namespace mr {
inline namespace math {

  using PackedNorm32 = uint32_t;

namespace details {

  Vec2f sign_non_zero(const Vec2f& v)
  {
    return Vec2f{v.x() >= 0, v.y() >= 0} * 2 - Vec2f{1};
  }

  Vec2f oct_wrap(const Vec2f& v)
  {
    return (Vec2f{1} - std::abs(Vec2f{v.y(), v.x()})) * sign_non_zero(v);
  }

  PackedNorm32 quantize(const Vec2f& norm)
  {
    const auto norm_trans = (norm * 0.5f + Vec2f{0.5}) * 1023.f;
    const auto norm_masked = static_cast<Vec2u>(norm_trans) & 0x3FF;
    return norm_masked.x() << 10 | norm_masked.y();
  }

  Vec2f dequantize(const PackedNorm32& norm)
  {
    const mr::Vec2f norm_masked{(norm >> 10) & 0x3FF, norm & 0x3FF};
    return norm_masked / 1023.0f * 2.0f - Vec2f{1.0f};
  }

} // namespace details

  // octahedron encoding

  PackedNorm32 pack_oct(const Norm3f& norm)
  {
    auto oct_proj = Vec2f{norm.x(), norm.y()} / (std::abs(norm.x()) + std::abs(norm.y()) + std::abs(norm.z()));
    if (norm.z() < 0) {
      oct_proj = mr::details::oct_wrap(oct_proj);
    }
    return mr::details::quantize(oct_proj);
  }

  Norm3f unpack_oct(const PackedNorm32& norm) {
    // TODO: add shader code for unpacking normals
    const auto oct_proj = mr::details::dequantize(norm);
    Vec3f vec{oct_proj, 1 - std::abs(oct_proj.x()) - std::abs(oct_proj.y())};
    if (vec.z() < 0) {
      vec = Vec3f{mr::details::oct_wrap(oct_proj), vec.z()};
    }
    return vec.normalized_unchecked();
  }

} // namespace math
} // namespace mr

#endif // __MR_PACKING_HPP_
