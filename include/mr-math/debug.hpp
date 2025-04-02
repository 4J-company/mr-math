#ifndef __MR_DEBUG_HPP_
#define __MR_DEBUG_HPP_

// This file provides template instantiations used by debug visualizers
// and is intended for debug builds only

#include "vec.hpp"

namespace mr {
inline namespace math {
namespace debug {

// following functions instantiate methods of template structs desired in debug visualization

inline namespace row {

  void instantiate_row_get(const auto& row) {
    volatile auto x = row.get(0); (void)x;
  }

#define MR_INSTANTIATE_ROW2(T) template void instantiate_row_get(const Row<T, 2>&)
#define MR_INSTANTIATE_ROW3(T) template void instantiate_row_get(const Row<T, 3>&)
#define MR_INSTANTIATE_ROW4(T) template void instantiate_row_get(const Row<T, 4>&)
#define MR_INSTANTIATE_ROW2_ROW3_ROW4(T) MR_INSTANTIATE_ROW2(T); MR_INSTANTIATE_ROW3(T); MR_INSTANTIATE_ROW4(T)

  // instantiate types
  MR_INSTANTIATE_ROW2_ROW3_ROW4(float);
  MR_INSTANTIATE_ROW2_ROW3_ROW4(double);
  MR_INSTANTIATE_ROW2_ROW3_ROW4(int);
  MR_INSTANTIATE_ROW2_ROW3_ROW4(uint32_t);

} // namespace row

inline namespace vec {

  void instantiate_vec_x_y_length(const auto& vec) {
    volatile auto x = vec.x(); (void)x;
    volatile auto y = vec.y(); (void)y;
    volatile auto l = vec.length(); (void)l;
  }

  void instantiate_vec_x_y_z_length(const auto& vec) {
    instantiate_vec_x_y_length(vec);
    volatile auto z = vec.z(); (void)z;
  }

  void instantiate_vec_x_y_z_w_length(const auto& vec) {
    instantiate_vec_x_y_z_length(vec);
    volatile auto w = vec.w(); (void)w;
  }

#define MR_INSTANTIATE_VEC2(T) template void instantiate_vec_x_y_length(const Vec2<T>&)
#define MR_INSTANTIATE_VEC3(T) template void instantiate_vec_x_y_z_length(const Vec3<T>&)
#define MR_INSTANTIATE_VEC4(T) template void instantiate_vec_x_y_z_w_length(const Vec4<T>&)
#define MR_INSTANTIATE_VEC2_VEC3_VEC4(T) MR_INSTANTIATE_VEC2(T); MR_INSTANTIATE_VEC3(T); MR_INSTANTIATE_VEC4(T)

  // instantiate types
  MR_INSTANTIATE_VEC2_VEC3_VEC4(float);
  MR_INSTANTIATE_VEC2_VEC3_VEC4(double);
  MR_INSTANTIATE_VEC2_VEC3_VEC4(int);
  MR_INSTANTIATE_VEC2_VEC3_VEC4(uint32_t);

} // namespace vec

inline namespace norm {

  void instantiate_norm_x_y(const auto& norm) {
    volatile auto x = norm.x(); (void)x;
    volatile auto y = norm.y(); (void)y;
  }

  void instantiate_norm_x_y_z(const auto& norm) {
    instantiate_norm_x_y(norm);
    volatile auto z = norm.z(); (void)z;
  }

  void instantiate_norm_x_y_z_w(const auto& norm) {
    instantiate_norm_x_y_z(norm);
    volatile auto w = norm.w(); (void)w;
  }

#define MR_INSTANTIATE_NORM2(T) template void instantiate_norm_x_y(const Norm2<T>&)
#define MR_INSTANTIATE_NORM3(T) template void instantiate_norm_x_y_z(const Norm3<T>&)
#define MR_INSTANTIATE_NORM4(T) template void instantiate_norm_x_y_z_w(const Norm4<T>&)
#define MR_INSTANTIATE_NORM2_NORM3_NORM4(T) MR_INSTANTIATE_NORM2(T); MR_INSTANTIATE_NORM3(T); MR_INSTANTIATE_NORM4(T)

  // instantiate types
  MR_INSTANTIATE_NORM2_NORM3_NORM4(float);
  MR_INSTANTIATE_NORM2_NORM3_NORM4(double);
  MR_INSTANTIATE_NORM2_NORM3_NORM4(int);
  MR_INSTANTIATE_NORM2_NORM3_NORM4(uint32_t);

} // namespace norm

inline namespace bounb_box {

  void instantiate_aabb_dimensions(const auto& aabb) {
    volatile auto d = aabb.dimensions(); (void)d;
  }

#define MR_INSTANTIATE_AABB(T) template void instantiate_aabb_dimensions(const AABB<T>&)

  // instantiate types
  MR_INSTANTIATE_AABB(float);
  MR_INSTANTIATE_AABB(double);
  MR_INSTANTIATE_AABB(int);
  MR_INSTANTIATE_AABB(uint32_t);

} // namespace norm

} // namespace debug
} // namespace math
} // namespace mr

#endif // __MR_DEBUG_HPP_
