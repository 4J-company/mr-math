file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.2/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=c8cdc32c03816538ce22781ed72964dc864b2a34a310d3b7104812a5ca2d835d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

find_package(Vc REQUIRED)

if (MR_MATH_ENABLE_BENCHMARK)
  find_package(benchmark REQUIRED)  
  set_target_properties(benchmark::benchmark PROPERTIES CXX_STANDARD 17)
endif()

if (MR_MATH_ENABLE_TESTING)
  find_package(GTest REQUIRED)
endif()

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.6.0")
