cmake_minimum_required(VERSION 3.27)
project(Math)

add_executable(${CMAKE_PROJECT_NAME} "tests/main.cpp")

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
