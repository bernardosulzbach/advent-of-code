cmake_minimum_required(VERSION 3.11)

project(advent-of-code-common-test)

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

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})
set(Boost_USE_MULTITHREADED ON)

add_library(
  advent-of-code OBJECT
  src/ArgumentParser.cpp
  src/ArgumentParser.hpp
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
  src/Math.hpp
  src/OrbitGraph.hpp
  src/Path.hpp
  src/Point.hpp
  src/Ranges.hpp
  src/Robot.cpp
  src/Robot.hpp
  src/Screen.hpp
  src/Text.cpp
  src/Text.hpp
  src/Types.hpp
  src/Vector.cpp
  src/Vector.hpp)

find_package(Threads REQUIRED)

include_directories(SYSTEM include)

add_executable(tests test/Tests.cpp $<TARGET_OBJECTS:advent-of-code>)

target_link_libraries(tests ${Boost_LIBRARIES})
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})
