cmake_minimum_required(VERSION 3.27)

include(cmake/CPM.cmake)
# google/benchmark
include(cmake/download_benchmark.cmake)
# google/googletest
# include(cmake/download_test.cmake)

# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -pedantic -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -pedantic -Wall -Wextra -ffast-math")

project(math)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 20)

target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# usage example
add_executable(${CMAKE_PROJECT_NAME}-tests "tests/main.cpp")
set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY CXX_STANDARD 20)
set_property(TARGET ${CMAKE_PROJECT_NAME}-tests PROPERTY CXX_STANDARD_REQUIRED 20)
target_link_libraries(${CMAKE_PROJECT_NAME}-tests PRIVATE
  ${CMAKE_PROJECT_NAME}
  benchmark
  )
