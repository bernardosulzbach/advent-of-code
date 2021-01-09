cmake_minimum_required(VERSION 3.11)

project(advent-of-code-common-test)

include(CMake/base.cmake)

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

include_directories(SYSTEM include)

add_executable(tests test/Tests.cpp $<TARGET_OBJECTS:advent-of-code>)

target_link_libraries(tests ${Boost_LIBRARIES})
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})
