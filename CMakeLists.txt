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
set(EXAMPLES_DIR 		${CMAKE_SOURCE_DIR}/examples)

# target names
set(LIBRARY_NAME 		${CMAKE_PROJECT_NAME})
set(EXAMPLES_NAME 		examples)

# -- Project Configuration --

include_directories(${INCLUDE_DIR})

# -- Target Definition --

# main static library
add_library(${LIBRARY_NAME} STATIC
  ${SRC_DIR}/expectation.cpp
  ${SRC_DIR}/expectation_order.cpp
  ${SRC_DIR}/spookshow.cpp)

# executable with examples
add_executable(${EXAMPLES_NAME} EXCLUDE_FROM_ALL
  ${EXAMPLES_DIR}/main.cpp)
target_link_libraries(${EXAMPLES_NAME} ${LIBRARY_NAME})

# -- Exports --

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
  set(SPOOKSHOW_LIBRARY_NAME ${LIBRARY_NAME} PARENT_SCOPE)
endif()
