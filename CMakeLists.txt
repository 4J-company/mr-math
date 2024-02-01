cmake_minimum_required(VERSION 3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

project(mr-math VERSION 1.0.0)

option(ENABLE_BENCHMARK "Whether benchmarks are downloaded and build" OFF)
option(PRESET_OPTIMIZED "Whether optimization flags are enabled at all" OFF)
option(PRESET_BENCHMARK "Whether maximum optimization flags are enabled" OFF)

set(MY_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -pedantic -Wall -Wextra")
if (PRESET_OPTIMIZED OR PRESET_BENCHMARK)
  set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -ffast-math")
endif()
if (PRESET_BENCHMARK)
  set(MY_CXX_FLAGS "${MY_CXX_FLAGS} -march=native")
endif()

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 23)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY COMPILE_FLAGS "${MY_CXX_FLAGS}")

target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

if (ENABLE_BENCHMARK)
  include(cmake/CPM.cmake)
  include(cmake/download_benchmark.cmake)

  add_executable(${CMAKE_PROJECT_NAME}-tests "tests/main.cpp")
  set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY CXX_STANDARD 23)
  set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY CXX_STANDARD_REQUIRED 23)
  set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY COMPILE_FLAGS "${MY_CXX_FLAGS}")
  target_link_libraries(${CMAKE_PROJECT_NAME}-tests PRIVATE
    benchmark
    mr-math
    )
endif()
