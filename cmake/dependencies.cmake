file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMFindPackage(
  NAME Vc
  GITHUB_REPOSITORY VcDevel/Vc
  GIT_TAG 1.4
)

if (ENABLE_BENCHMARK)
  CPMFindPackage(
    NAME benchmark
    GITHUB_REPOSITORY google/benchmark
    VERSION 1.5.2
    OPTIONS "BENCHMARK_ENABLE_TESTING Off"
  )
endif()

if (ENABLE_TESTING)
  CPMFindPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    VERSION 1.14.0
  )
endif()

if(googletest_ADDED)
  # enable c++11 to avoid compilation errors
  set_target_properties(googletest PROPERTIES CXX_STANDARD 11)
endif()
