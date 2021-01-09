cmake_minimum_required(VERSION 3.11)

project(advent-of-code-common-test)

include(CMake/base.cmake)

add_library(
  advent-of-code OBJECT
  src/ArgumentParser.cpp
  src/ArgumentParser.hpp
  src/Bit.hpp
  src/BitMask.cpp
  src/BitMask.hpp
  src/Body.cpp
  src/Body.hpp
  src/BugScan.hpp
  src/Cycle.hpp
  src/DirectedGraph.cpp
  src/DirectedGraph.hpp
  src/Direction.cpp
  src/Direction.hpp
  src/Filesystem.cpp
  src/Filesystem.hpp
  src/IO.hpp
  src/Intcode.cpp
  src/Intcode.hpp
  src/IntcodeState.hpp
  src/Interval.hpp
  src/Math.hpp
  src/OrbitGraph.hpp
  src/Path.hpp
  src/Point.hpp
  src/Pose.cpp
  src/Pose.hpp
  src/Ranges.hpp
  src/Screen.hpp
  src/Text.cpp
  src/Text.hpp
  src/Types.hpp
  src/Vector.cpp
  src/Vector.hpp)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)

include_directories(SYSTEM include)

add_executable(tests test/Tests.cpp $<TARGET_OBJECTS:advent-of-code>)

target_link_libraries(tests ${Boost_LIBRARIES})
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})
