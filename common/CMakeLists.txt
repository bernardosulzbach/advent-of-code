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
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak"
  )
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak"
  )
endif()

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})
set(Boost_USE_MULTITHREADED ON)

add_library(
  advent-of-code OBJECT
  src/Types.hpp
  src/ArgumentParser.hpp
  src/ArgumentParser.cpp
  src/Intcode.cpp
  src/Intcode.hpp
  src/Point.hpp
  src/Path.hpp
  src/Math.hpp
  src/Math.cpp
  src/Vector.hpp
  src/Vector.cpp
  src/OrbitGraph.hpp
  src/OrbitGraph.cpp
  src/IntcodeState.hpp
  src/IntcodeState.cpp
  src/Direction.hpp
  src/Direction.cpp
  src/Robot.hpp
  src/Robot.cpp
  src/Body.hpp
  src/Body.cpp
  src/Screen.hpp
  src/Screen.cpp
  src/Text.hpp
  src/Text.cpp
  src/Cycle.hpp
  src/Cycle.cpp
  src/DirectedGraph.hpp
  src/DirectedGraph.cpp
  src/StandardInput.hpp
  src/StandardInput.cpp
  src/BugScan.cpp
  src/BugScan.hpp
  src/Filesystem.cpp
  src/Filesystem.hpp
  src/Ranges.hpp)

find_package(Threads REQUIRED)

include_directories(SYSTEM include)

add_executable(tests test/Tests.cpp $<TARGET_OBJECTS:advent-of-code>)

target_link_libraries(tests ${Boost_LIBRARIES})
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})
