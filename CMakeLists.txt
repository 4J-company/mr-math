cmake_minimum_required(VERSION 3.27)

project(mr-math VERSION 1.0.0)

option(ENABLE_BENCHMARK "Whether benchmarks are downloaded and built" OFF)
option(ENABLE_TESTING   "Whether tests are downloaded and built" OFF)
option(PRESET_OPTIMIZED "Whether optimization flags are enabled at all" OFF)
option(PRESET_BENCHMARK "Whether maximum optimization flags are enabled" OFF)
option(PRESET_PROFILING "Whether profiling flags are enabled" OFF)

include(cmake/dependencies.cmake)

set(MY_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -pedantic -Wall -Wextra")
if (PRESET_OPTIMIZED OR PRESET_BENCHMARK)
  set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -ffast-math")
endif()
if (PRESET_BENCHMARK)
  set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -march=native")
endif()
if (PRESET_PROFILING)
  set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -lprofiler -ltcmalloc")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MR-MATH-LIB ${CMAKE_PROJECT_NAME}-lib)

add_library(${MR-MATH-LIB} INTERFACE
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
target_include_directories(${MR-MATH-LIB} INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${MR-MATH-LIB} INTERFACE Vc)

if (ENABLE_BENCHMARK)
  set(MR-MATH-BENCH ${CMAKE_PROJECT_NAME}-bench)

  add_executable(${MR-MATH-BENCH} "bench/main.cpp")

  target_compile_options(${MR-MATH-BENCH} PRIVATE ${MY_CXX_FLAGS})
  target_include_directories(${MR-MATH-BENCH} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
  target_link_libraries(${MR-MATH-BENCH} PRIVATE benchmark ${CMAKE_PROJECT_NAME}-lib)
endif()
