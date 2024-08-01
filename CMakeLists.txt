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

option(MR_MATH_ENABLE_BENCHMARK "Whether benchmarks are downloaded and build"    OFF)
option(MR_MATH_ENABLE_TESTING   "Whether tests are downloaded and build"         OFF)
option(MR_MATH_PRESET_OPTIMIZED "Whether optimization flags are enabled at all"  OFF)
option(MR_MATH_PRESET_BENCHMARK "Whether maximum optimization flags are enabled" OFF)
option(MR_MATH_PRESET_PROFILING "Whether profiling flags are enabled"            OFF)

if (NOT MSVC)
  set(MY_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
  if (PRESET_OPTIMIZED OR PRESET_BENCHMARK)
    set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -ffast-math")
  endif()
  if (PRESET_BENCHMARK)
    set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -march=native")
  endif()
  if (PRESET_PROFILING)
    set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -lprofiler -ltcmalloc")
  endif()
endif()

include(cmake/dependencies.cmake)

set(MR_MATH_LIB ${CMAKE_PROJECT_NAME}-lib)
add_library(${MR_MATH_LIB} INTERFACE
    mr-math/camera.hpp
    mr-math/def.hpp
    mr-math/matr.hpp
    mr-math/norm.hpp
    mr-math/operators.hpp
    mr-math/rot.hpp
    mr-math/row.hpp
    mr-math/units.hpp
    mr-math/vec.hpp
    mr-math/math.hpp
)
target_include_directories(${MR_MATH_LIB} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(${MR_MATH_LIB} INTERFACE Vc)
target_compile_features(${MR_MATH_LIB} INTERFACE cxx_std_23)

if (MR_MATH_ENABLE_BENCHMARK)
  add_executable(${MR_MATH_BENCH_NAME} "bench/main.cpp")
  set_property(TARGET ${MR_MATH_BENCH_NAME} PROPERTY COMPILE_FLAGS "${MY_CXX_FLAGS}")
  target_link_libraries(${MR_MATH_BENCH_NAME} PRIVATE benchmark ${MR_MATH_LIB_NAME})
endif()

if (MR_MATH_ENABLE_TESTING)
  include(GoogleTest)
  add_executable(${MR_MATH_TESTS_NAME} "tests/main.cpp")
  target_link_libraries(${MR_MATH_TESTS_NAME} PRIVATE gtest_main ${MR_MATH_LIB})
  gtest_discover_tests(${MR_MATH_TESTS_NAME})
endif()
