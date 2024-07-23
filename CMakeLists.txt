cmake_minimum_required(VERSION 3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mr-math VERSION 1.0.0)

option(ENABLE_BENCHMARK "Whether benchmarks are downloaded and build" OFF)
option(ENABLE_TESTING   "Whether tests are downloaded and build" OFF)
option(PRESET_OPTIMIZED "Whether optimization flags are enabled at all" OFF)
option(PRESET_BENCHMARK "Whether maximum optimization flags are enabled" OFF)
option(PRESET_PROFILING "Whether profiling flags are enabled" OFF)

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

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
target_include_directories(${MR_MATH_LIB} INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${MR_MATH_LIB} INTERFACE Vc)

if (ENABLE_BENCHMARK)
  set(MR_MATH_BENCH ${CMAKE_PROJECT_NAME}-bench)
  add_executable(${MR_MATH_BENCH} "bench/main.cpp")

  set_property(TARGET ${MR_MATH_BENCH} PROPERTY COMPILE_FLAGS "${MY_CXX_FLAGS}")

  target_include_directories(${MR_MATH_BENCH} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
  target_link_libraries(${MR_MATH_BENCH} PRIVATE benchmark ${MR_MATH_LIB})
endif()

if (ENABLE_TESTING)
  include(CTest)

  set(MR_MATH_TEST ${CMAKE_PROJECT_NAME}-tests)
  add_executable(${MR_MATH_TEST} "tests/main.cpp")

  set_property(TARGET ${MR_MATH_TEST} PROPERTY COMPILE_FLAGS "${MY_CXX_FLAGS}")

  target_include_directories(${MR_MATH_TEST} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
  target_link_libraries(${MR_MATH_TEST} PRIVATE gtest_main ${MR_MATH_LIB})

  add_test(NAME ${MR_MATH_TEST} COMMAND ${MR_MATH_TEST})
endif()
