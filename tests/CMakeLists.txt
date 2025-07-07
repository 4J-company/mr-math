include(GoogleTest)
add_executable(${MR_MATH_TESTS_NAME} "misc.cpp" "matr.cpp" "vec.cpp" "quat.cpp" "norm.cpp")
target_link_libraries(${MR_MATH_TESTS_NAME} PRIVATE GTest::gtest_main ${MR_MATH_LIB_NAME})
gtest_discover_tests(${MR_MATH_TESTS_NAME})
