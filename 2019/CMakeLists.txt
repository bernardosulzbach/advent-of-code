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
  set(
    CMAKE_CXX_FLAGS

    "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak"
    )
  set(
    CMAKE_EXE_LINKER_FLAGS_DEBUG

    "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak"
    )
endif()

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})
set(Boost_USE_MULTITHREADED ON)

add_library(advent-of-code OBJECT
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
            src/StandardInput.cpp)

find_package(Threads REQUIRED)

add_executable(tests test/Tests.cpp $<TARGET_OBJECTS:advent-of-code>)

target_link_libraries(tests ${Boost_LIBRARIES})
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})

add_executable(runner src/Runner.cpp)
target_link_libraries(runner ${Boost_LIBRARIES})
target_link_libraries(runner ${CMAKE_THREAD_LIBS_INIT})

add_executable(01 src/01.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(02 src/02.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(03 src/03.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(04 src/04.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(05 src/05.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(06 src/06.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(07 src/07.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(08 src/08.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(09 src/09.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(10 src/10.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(11 src/11.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(12 src/12.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(13 src/13.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(14 src/14.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(15 src/15.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(16 src/16.cpp $<TARGET_OBJECTS:advent-of-code>)
add_executable(17 src/17.cpp $<TARGET_OBJECTS:advent-of-code>)
