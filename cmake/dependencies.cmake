file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.2/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=c8cdc32c03816538ce22781ed72964dc864b2a34a310d3b7104812a5ca2d835d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMFindPackage(
  NAME Vc
  GITHUB_REPOSITORY 4J-company/Vc
  GIT_TAG 1.4
  OPTIONS
    "USE_CCACHE ON"
)

if (MR_MATH_ENABLE_BENCHMARK)
  CPMFindPackage(
    NAME benchmark
    GITHUB_REPOSITORY google/benchmark
    GIT_TAG main
    OPTIONS
      "BENCHMARK_ENABLE_TESTING OFF"
  )
endif()

if (benchmark_ADDED)
  # patch benchmark target
  set_target_properties(benchmark PROPERTIES CXX_STANDARD 17)
endif()

if (MR_MATH_ENABLE_TESTING)
  CPMFindPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG main
    OPTIONS
      "INSTALL_GTEST OFF"
      "gtest_force_shared_crt ON"
  )
endif()

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.6.0")
