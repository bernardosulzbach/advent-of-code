cmake_minimum_required(VERSION 3.11)

project(advent-of-code-2020)

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
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak"
  )
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak"
  )
endif()

add_library(
  advent-of-code-common OBJECT
  ../common/src/Types.hpp
  ../common/src/ArgumentParser.hpp
  ../common/src/ArgumentParser.cpp
  ../common/src/Intcode.cpp
  ../common/src/Intcode.hpp
  ../common/src/Point.hpp
  ../common/src/Path.hpp
  ../common/src/Math.hpp
  ../common/src/Math.cpp
  ../common/src/Vector.hpp
  ../common/src/Vector.cpp
  ../common/src/OrbitGraph.hpp
  ../common/src/OrbitGraph.cpp
  ../common/src/IntcodeState.hpp
  ../common/src/IntcodeState.cpp
  ../common/src/Direction.hpp
  ../common/src/Direction.cpp
  ../common/src/Robot.hpp
  ../common/src/Robot.cpp
  ../common/src/Body.hpp
  ../common/src/Body.cpp
  ../common/src/Screen.hpp
  ../common/src/Screen.cpp
  ../common/src/Text.hpp
  ../common/src/Text.cpp
  ../common/src/Cycle.hpp
  ../common/src/Cycle.cpp
  ../common/src/DirectedGraph.hpp
  ../common/src/DirectedGraph.cpp
  ../common/src/StandardInput.hpp
  ../common/src/StandardInput.cpp
  ../common/src/BugScan.cpp
  ../common/src/BugScan.hpp
  ../common/src/Filesystem.cpp
  ../common/src/Filesystem.hpp
  ../common/src/Ranges.hpp)

include_directories("../common/src")

add_executable(01 src/01.cpp $<TARGET_OBJECTS:advent-of-code-common>)
