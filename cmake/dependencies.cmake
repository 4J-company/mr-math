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
  GIT_TAG 1.4.1
)

if (MR_MATH_ENABLE_BENCHMARK)
  CPMFindPackage(
    NAME benchmark
    GITHUB_REPOSITORY google/benchmark
    GIT_TAG 1.14.0
  )
endif()

if (MR_MATH_ENABLE_TESTING)
  CPMFindPackage(
    NAME GTest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG 1.9.1
  )
endif()

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.6.0")
