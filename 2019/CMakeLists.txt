cmake_minimum_required(VERSION 3.11)

project(advent-of-code)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_definitions(-Wall)
add_definitions(-Wextra)
add_definitions(-Wshadow)
add_definitions(-Werror)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-DDEBUGGING)
endif()

add_library(
  advent-of-code OBJECT
  src/Types.hpp
  src/ArgumentParser.hpp
  src/ArgumentParser.cpp
  src/Intcode.cpp
  src/Intcode.hpp
  src/Point.hpp
  src/Path.hpp
  src/OrbitGraph.hpp
  src/OrbitGraph.cpp
  src/IntcodeState.hpp
  src/IntcodeState.cpp
  src/StandardInput.hpp
  src/StandardInput.cpp)

add_executable(01 src/01.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(02 src/02.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(03 src/03.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(04 src/04.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(05 src/05.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(06 src/06.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(07 src/07.cpp $<TARGET_OBJECTS:advent-of-code>)
