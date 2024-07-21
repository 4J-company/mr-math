cmake_minimum_required(VERSION 3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mr-math VERSION 1.0.0)

option(ENABLE_BENCHMARK "Whether benchmarks are downloaded and build" OFF)
option(ENABLE_TESTING   "Whether tests are downloaded and build" OFF)
option(PRESET_OPTIMIZED "Whether optimization flags are enabled at all" OFF)
option(PRESET_BENCHMARK "Whether maximum optimization flags are enabled" OFF)
option(PRESET_PROFILING "Whether profiling flags are enabled" OFF)

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

include(cmake/dependencies.cmake)

add_library(${CMAKE_PROJECT_NAME}-lib INTERFACE
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
target_include_directories(${CMAKE_PROJECT_NAME}-lib INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME}-lib INTERFACE Vc)

if (ENABLE_BENCHMARK)
  add_executable(${CMAKE_PROJECT_NAME}-bench "bench/main.cpp")

  set_property(TARGET ${CMAKE_PROJECT_NAME}-bench PROPERTY CXX_STANDARD 23)
  set_property(TARGET ${CMAKE_PROJECT_NAME}-bench PROPERTY CXX_STANDARD_REQUIRED 23)
  set_property(TARGET ${CMAKE_PROJECT_NAME}-bench PROPERTY COMPILE_FLAGS "${MY_CXX_FLAGS}")

  target_include_directories(${CMAKE_PROJECT_NAME}-bench PRIVATE ${CMAKE_CURRENT_LIST_DIR})
  target_link_libraries(${CMAKE_PROJECT_NAME}-bench PRIVATE benchmark ${CMAKE_PROJECT_NAME}-lib)
endif()

if (ENABLE_TESTING)
  add_executable(${CMAKE_PROJECT_NAME}-tests "tests/main.cpp")

  set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY CXX_STANDARD 23)
  set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY CXX_STANDARD_REQUIRED 23)
  set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY COMPILE_FLAGS "${MY_CXX_FLAGS}")

  target_include_directories(${CMAKE_PROJECT_NAME}-tests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
  target_link_libraries(${CMAKE_PROJECT_NAME}-tests PRIVATE gtest_main ${CMAKE_PROJECT_NAME}-lib)
endif()
