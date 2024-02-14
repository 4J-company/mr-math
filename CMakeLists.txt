cmake_minimum_required(VERSION 3.27)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

project(mr-math VERSION 1.0.0)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 23)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-generate -march=native -fno-exceptions -pedantic -Wall -Wextra -ffast-math")

target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

if (ENABLE_BENCHMARK)
  include(cmake/CPM.cmake)
  include(cmake/download_benchmark.cmake)

  add_executable(${CMAKE_PROJECT_NAME}-tests "tests/main.cpp")
  set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY CXX_STANDARD 23)
  set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY CXX_STANDARD_REQUIRED 23)
  set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -march=native -fno-exceptions -pedantic -Wall -Wextra -ffast-math")
  target_link_libraries(${CMAKE_PROJECT_NAME}-tests PRIVATE
    ${CMAKE_PROJECT_NAME}
    benchmark
    mr-math
    )
endif()
