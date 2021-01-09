cmake_minimum_required(VERSION 3.11)

project(advent-of-code-2020)

include(../common/CMake/base.cmake)

add_library(advent-of-code-common OBJECT ../common/src/ArgumentParser.cpp ../common/src/BitMask.cpp ../common/src/Direction.cpp ../common/src/Pose.cpp ../common/src/Text.cpp)

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
add_executable(12 src/12.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(13 src/13.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(14 src/14.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(15 src/15.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(16 src/16.cpp $<TARGET_OBJECTS:advent-of-code-common>)
add_executable(17 src/17.cpp $<TARGET_OBJECTS:advent-of-code-common>)
