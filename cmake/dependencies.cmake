file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMFindPackage(
  NAME Vc
  GITHUB_REPOSITORY 4J-company/Vc
  GIT_TAG 1.4
  OPTIONS
    "USE_CCACHE ON"
)

if (ENABLE_BENCHMARK)
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

if (ENABLE_TESTING)
  CPMFindPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    GIT_TAG main
    OPTIONS
      "INSTALL_GTEST OFF"
      "gtest_force_shared_crt ON"
  )
endif()

