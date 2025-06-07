add_executable(${MR_MATH_BENCH_NAME} "bench.cpp")

# Options for disabling specific benchmarks
option(MR_MATH_ENABLE_VEC_BENCH   "Whether vector benchmarks are performed"     ON)
option(MR_MATH_ENABLE_MATR_BENCH  "Whether matrix benchmarks are performed"     ON)
option(MR_MATH_ENABLE_QUAT_BENCH  "Whether quaternion benchmarks are performed" ON)
option(MR_MATH_ENABLE_CAM_BENCH   "Whether camera benchmarks are performed"     ON)
option(MR_MATH_ENABLE_AABB_BENCH  "Whether bound box benchmarks are performed"  ON)
option(MR_MATH_ENABLE_COLOR_BENCH "Whether color benchmarks are performed"      ON)

configure_file(bench_options.hpp.in config/bench_options.hpp)
target_include_directories(${MR_MATH_BENCH_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/config") # to include bench_options.hpp

if (NOT MSVC)
  set(MR_MATH_BENCH_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -march=native")
  if (MR_MATH_EXTRA_OPTIMIZED)
    set(MR_MATH_BENCH_CXX_FLAGS "${MR_MATH_BENCH_CXX_FLAGS} -ffast-math")
  endif()
  if (MR_MATH_ENABLE_PROFILING)
    set(MR_MATH_BENCH_CXX_FLAGS "${MR_MATH_BENCH_CXX_FLAGS} -lprofiler -ltcmalloc")
  endif()
else()
  set(MR_MATH_BENCH_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
  if (MR_MATH_EXTRA_OPTIMIZED)
    set(MR_MATH_BENCH_CXX_FLAGS "${MR_MATH_BENCH_CXX_FLAGS} /fp:fast")
  endif()
endif()

set_property(TARGET ${MR_MATH_BENCH_NAME} PROPERTY COMPILE_FLAGS "${MR_MATH_BENCH_CXX_FLAGS}")
target_link_libraries(${MR_MATH_BENCH_NAME} PRIVATE benchmark::benchmark ${MR_MATH_LIB_NAME})

set_property(TARGET ${MR_MATH_BENCH_NAME} PROPERTY COMPILE_FLAGS "${MR_MATH_BENCH_CXX_FLAGS}")
target_link_libraries(${MR_MATH_BENCH_NAME} PRIVATE benchmark ${MR_MATH_LIB_NAME})
