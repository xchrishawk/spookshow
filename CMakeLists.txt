# Spookshow CMakeLists.txt

# -- Project Configuration --

cmake_minimum_required(VERSION 3.4)
project(spookshow)

# toolchain flags
set(CMAKE_CXX_FLAGS 		"-std=gnu++11 -Wall -Wpedantic")
set(CMAKE_CXX_DEBUG_FLAGS 	"-g")
set(CMAKE_CXX_RELEASE_FLAGS	"-O2 -s -Werror")

# working directories
set(SRC_DIR 			${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR 		${CMAKE_SOURCE_DIR}/include)
set(TESTS_DIR			${CMAKE_SOURCE_DIR}/tests)
set(EXAMPLES_DIR 		${CMAKE_SOURCE_DIR}/examples)

# target names
set(LIBRARY_NAME 		${CMAKE_PROJECT_NAME})
set(TESTS_NAME			tests)
set(EXAMPLES_NAME 		examples)

# include directories
include_directories(${INCLUDE_DIR})

# -- Third Party Libraries --

# Google Test (for unit testing)
find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

# -- Target Definition --

# main static library
add_library(${LIBRARY_NAME} STATIC
  ${SRC_DIR}/expectation.cpp
  ${SRC_DIR}/expectation_order.cpp
  ${SRC_DIR}/spookshow.cpp)

# unit tests (if GTest is found)
if (${GTEST_FOUND})

  enable_testing()

  add_executable(${TESTS_NAME} EXCLUDE_FROM_ALL
    ${TESTS_DIR}/main.cpp
    ${TESTS_DIR}/expectation_order_tests.cpp
    ${TESTS_DIR}/expectation_tests.cpp
    ${TESTS_DIR}/method_tests.cpp)
  target_link_libraries(${TESTS_NAME}
    ${LIBRARY_NAME}
    ${GTEST_BOTH_LIBRARIES}
    pthread)

  add_test(all ${TESTS_NAME})

endif()

# examples executable
add_executable(${EXAMPLES_NAME} EXCLUDE_FROM_ALL
  ${EXAMPLES_DIR}/main.cpp)
target_link_libraries(${EXAMPLES_NAME}
  ${LIBRARY_NAME})

# -- Exports --

# Export library information to the parent scope, if there is one
get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
  set(SPOOKSHOW_LIBRARY_NAME ${LIBRARY_NAME} PARENT_SCOPE)
  set(SPOOKSHOW_INCLUDE_DIRS ${INCLUDE_DIR} PARENT_SCOPE)
endif()
