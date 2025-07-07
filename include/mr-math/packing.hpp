#ifndef __MR_PACKING_HPP_
#define __MR_PACKING_HPP_

#include "norm.hpp"
#include "vec.hpp"

namespace mr {
inline namespace math {

  using PackedNorm32 = uint32_t;

  using PackedNorm16 = uint16_t;

  struct PackedNorm24 {
    PackedNorm24() = default;

    constexpr explicit PackedNorm24(uint32_t value) noexcept {
      _data[0] = value & 0xFF;
      _data[1] = (value >> 8) & 0xFF;
      _data[2] = (value >> 16) & 0xFF;
    }

    constexpr explicit operator uint32_t() const noexcept {
      return
        (static_cast<uint32_t>(_data[2]) << 16) |
        (static_cast<uint32_t>(_data[1]) << 8) |
         static_cast<uint32_t>(_data[0]);
    }

    constexpr explicit operator const uint8_t* () const noexcept {
      return _data.data();
    }

    constexpr explicit operator uint8_t* () noexcept {
      return _data.data();
    }

  private:
    std::array<uint8_t, 3> _data;
  };

namespace details {

  Vec2f sign_non_zero(const Vec2f& v)
  {
    return Vec2f{v.x() >= 0, v.y() >= 0} * 2 - Vec2f{1};
  }

  Vec2f oct_wrap(const Vec2f& v)
  {
    return (Vec2f{1} - std::abs(Vec2f{v.y(), v.x()})) * sign_non_zero(v);
  }

  PackedNorm32 quantize32(const Vec2f& norm)
  {
    const auto norm_trans = (norm * 0.5f + Vec2f{ 0.5 }) * 1023.f;
    const auto norm_masked = static_cast<Vec2u>(norm_trans) & 0x3FF;
    return norm_masked.x() << 10 | norm_masked.y();
  }

  Vec2f dequantize32(const PackedNorm32& norm)
  {
    const mr::Vec2f norm_masked{ (norm >> 10) & 0x3FF, norm & 0x3FF };
    return norm_masked / 1023.0f * 2.0f - Vec2f{ 1.0f };
  }

  PackedNorm24 quantize24(const Vec2f& norm)
  {
    const auto norm_trans = (norm * 0.5f + Vec2f{ 0.5 }) * 511.f;
    const auto norm_masked = static_cast<Vec2u>(norm_trans) & 0x1FF;
    return PackedNorm24(norm_masked.x() << 9 | norm_masked.y());
  }

  Vec2f dequantize24(const PackedNorm24& norm)
  {
    const uint32_t norm32 = static_cast<uint32_t>(norm);
    const mr::Vec2f norm_masked{ (norm32 >> 9) & 0x1FF, norm32 & 0x1FF };
    return norm_masked / 511.0f * 2.0f - Vec2f{ 1.0f };
  }

  PackedNorm16 quantize16(const Vec2f& norm)
  {
    const auto norm_trans = (norm * 0.5f + Vec2f{ 0.5 }) * 31.f;
    const auto norm_masked = static_cast<Vec2u>(norm_trans) & 0x1F;
    return norm_masked.x() << 5 | norm_masked.y();
  }

  Vec2f dequantize16(const PackedNorm16& norm)
  {
    const mr::Vec2f norm_masked{ (norm >> 5) & 0x1F, norm & 0x1F };
    return norm_masked / 31.0f * 2.0f - Vec2f{ 1.0f };
  }

  // octahedron encoding

  Vec2f pack_oct_impl(const Norm3f& norm)
  {
    auto oct_proj = Vec2f{norm.x(), norm.y()} / (std::abs(norm.x()) + std::abs(norm.y()) + std::abs(norm.z()));
    if (norm.z() < 0) {
      oct_proj = mr::details::oct_wrap(oct_proj);
    }
    return oct_proj;
  }

  Norm3f unpack_oct_impl(const Vec2f& norm) {
    // TODO: add shader code for unpacking normals
    Vec3f vec{ norm, 1 - std::abs(norm.x()) - std::abs(norm.y())};
    if (vec.z() < 0) {
      vec = Vec3f{mr::details::oct_wrap(norm), vec.z()};
    }
    return vec.normalized_unchecked();
  }

} // namespace details

  inline PackedNorm32 pack_oct32(const Norm3f& norm)
  {
    return math::details::quantize32(math::details::pack_oct_impl(norm));
  }

  inline Norm3f unpack_oct32(const PackedNorm32& norm)
  {
    return math::details::unpack_oct_impl(math::details::dequantize32(norm));
  }

  inline PackedNorm24 pack_oct24(const Norm3f& norm)
  {
    return math::details::quantize24(math::details::pack_oct_impl(norm));
  }

  inline Norm3f unpack_oct24(const PackedNorm24& norm)
  {
    return math::details::unpack_oct_impl(math::details::dequantize24(norm));
  }

  inline PackedNorm16 pack_oct16(const Norm3f& norm)
  {
    return math::details::quantize16(math::details::pack_oct_impl(norm));
  }

  inline Norm3f unpack_oct16(const PackedNorm16& norm)
  {
    return math::details::unpack_oct_impl(math::details::dequantize16(norm));
  }

} // namespace math
} // namespace mr

#endif // __MR_PACKING_HPP_
