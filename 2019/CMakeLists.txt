cmake_minimum_required(VERSION 3.11)

project(advent-of-code-2019)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-Wall)
  add_definitions(-Wextra)
  add_definitions(-Wshadow)
endif()

add_definitions(-Werror)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-DDEBUGGING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak")
endif()

add_library(
  advent-of-code-common OBJECT
  ../common/src/ArgumentParser.cpp
  ../common/src/ArgumentParser.hpp
  ../common/src/Body.cpp
  ../common/src/Body.hpp
  ../common/src/BugScan.hpp
  ../common/src/Cycle.hpp
  ../common/src/DirectedGraph.cpp
  ../common/src/DirectedGraph.hpp
  ../common/src/Direction.cpp
  ../common/src/Direction.hpp
  ../common/src/Filesystem.cpp
  ../common/src/Filesystem.hpp
  ../common/src/IO.hpp
  ../common/src/Intcode.cpp
  ../common/src/Intcode.hpp
  ../common/src/IntcodeState.hpp
  ../common/src/Math.hpp
  ../common/src/OrbitGraph.hpp
  ../common/src/Path.hpp
  ../common/src/Point.hpp
  ../common/src/Pose.cpp
  ../common/src/Screen.hpp
  ../common/src/Text.cpp
  ../common/src/Text.hpp
  ../common/src/Types.hpp
  ../common/src/Vector.cpp
  ../common/src/Vector.hpp)

include_directories("../common/src")

include_directories(SYSTEM "include")

add_executable(01 src/01.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(02 src/02.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(03 src/03.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(04 src/04.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(05 src/05.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(06 src/06.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(07 src/07.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(08 src/08.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(09 src/09.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(10 src/10.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(11 src/11.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(12 src/12.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(13 src/13.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(14 src/14.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(15 src/15.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(16 src/16.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(17 src/17.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(19 src/19.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(21 src/21.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(24 src/24.cpp $<TARGET_OBJECTS:advent-of-code-common>)

add_executable(runner src/Runner.cpp $<TARGET_OBJECTS:advent-of-code-common>)
