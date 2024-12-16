#ifndef __debug_hpp_
#define __debug_hpp_

// This file provides template instantiations used by debug visualizers
// and is intended for debug builds only

#include "vec.hpp"

namespace mr {
namespace debug {

  // following functions instantiate methods of template structs desired in debug visualization

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

} // namespace debug
} // namespace mr

#endif // __debug_hpp_

