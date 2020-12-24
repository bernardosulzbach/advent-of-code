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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize=leak")
endif()

add_library(advent-of-code-common OBJECT ../common/src/ArgumentParser.hpp ../common/src/ArgumentParser.cpp ../common/src/Math.hpp ../common/src/Ranges.hpp ../common/src/Text.cpp)

include_directories("../common/src")

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
