cmake_minimum_required(VERSION 3.13) # We use add_link_options

project(advent-of-code-common-test)

include(CMake/Settings.cmake)
include(CMake/Warnings.cmake)

if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message(FATAL_ERROR "${CMAKE_PROJECT_NAME} should only be built in Debug mode. We want all asserts() and sanitizers to be on.")
endif()

add_library(
  advent-of-code OBJECT
  src/ArgumentParser.cpp
  src/BitMask.cpp
  src/Body.cpp
  src/DirectedGraph.cpp
  src/Direction.cpp
  src/Intcode.cpp
  src/Pose.cpp
  src/Text.cpp
  src/Vector.cpp)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)

include_directories(include)

add_executable(tests test/Tests.cpp $<TARGET_OBJECTS:advent-of-code>)

target_link_libraries(tests ${Boost_LIBRARIES})
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})
