CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  VERSION 1.14.0
)

if(googletest_ADDED)
  # enable c++11 to avoid compilation errors
  set_target_properties(googletest PROPERTIES CXX_STANDARD 11)
endif()
