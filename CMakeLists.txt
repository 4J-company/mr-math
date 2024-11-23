cmake_minimum_required(VERSION 3.27)

set(MR_MATH_PROJECT_NAME mr-math)
set(MR_MATH_LIB_NAME     mr-math-lib)
set(MR_MATH_BENCH_NAME   mr-math-bench)
set(MR_MATH_TESTS_NAME   mr-math-tests)

project(
  ${MR_MATH_PROJECT_NAME}
  VERSION 1.0.0
  LANGUAGES CXX
)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mr-math VERSION 1.0.0)

option(MR_MATH_ENABLE_BENCHMARK "Whether benchmarks are downloaded and build"    OFF)
option(MR_MATH_ENABLE_TESTING   "Whether tests are downloaded and build"         OFF)
option(MR_MATH_PRESET_OPTIMIZED "Whether optimization flags are enabled at all"  OFF)
option(MR_MATH_PRESET_BENCHMARK "Whether maximum optimization flags are enabled" OFF)
option(MR_MATH_PRESET_PROFILING "Whether profiling flags are enabled"            OFF)

include(cmake/dependencies.cmake)

add_library(${MR_MATH_LIB_NAME} INTERFACE
  include/mr-math/camera.hpp
  include/mr-math/def.hpp
  include/mr-math/matr.hpp
  include/mr-math/norm.hpp
  include/mr-math/operators.hpp
  include/mr-math/rot.hpp
  include/mr-math/row.hpp
  include/mr-math/units.hpp
  include/mr-math/vec.hpp
  include/mr-math/math.hpp
  include/mr-math/bound_box.hpp
)

target_include_directories(${MR_MATH_LIB_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${MR_MATH_LIB_NAME} INTERFACE Vc)
target_compile_features(${MR_MATH_LIB_NAME} INTERFACE cxx_std_23)

if (MR_MATH_ENABLE_BENCHMARK)
  add_executable(${MR_MATH_BENCH_NAME} "bench/main.cpp")
  if (NOT MSVC)
    set(MR_MATH_BENCH_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
    if (MR_MATH_PRESET_OPTIMIZED OR MR_MATH_PRESET_BENCHMARK)
      set(MR_MATH_BENCH_CXX_FLAGS "${MR_MATH_BENCH_CXX_FLAGS} -ffast-math")
    endif()
    if (MR_MATH_PRESET_BENCHMARK)
      set(MR_MATH_BENCH_CXX_FLAGS "${MR_MATH_BENCH_CXX_FLAGS} -march=native")
    endif()
    if (MR_MATH_PRESET_PROFILING)
      set(MR_MATH_BENCH_CXX_FLAGS "${MR_MATH_BENCH_CXX_FLAGS} -lprofiler -ltcmalloc")
    endif()
  endif()
  set_property(TARGET ${MR_MATH_BENCH_NAME} PROPERTY COMPILE_FLAGS "${MR_MATH_BENCH_CXX_FLAGS}")
  target_link_libraries(${MR_MATH_BENCH_NAME} PRIVATE benchmark ${MR_MATH_LIB_NAME})
endif()

if (MR_MATH_ENABLE_TESTING)
  include(GoogleTest)
  add_executable(${MR_MATH_TESTS_NAME} "tests/main.cpp")
  target_link_libraries(${MR_MATH_TESTS_NAME} PRIVATE gtest_main ${MR_MATH_LIB_NAME})
  gtest_discover_tests(${MR_MATH_TESTS_NAME})
endif()

packageProject(
  NAME ${MR_MATH_LIB_NAME}
  VERSION ${PROJECT_VERSION}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  INCLUDE_HEADER_PATTERN "*.hpp"
  DEPENDENCIES "Vc 1.4"
  NAMESPACE mr
  COMPATIBILITY AnyNewerVersion # supported values: `AnyNewerVersion|SameMajorVersion|SameMinorVersion|ExactVersion`
  ARCH_INDEPENDENT YES
  CPACK NO
)
