#!/usr/bin/env bash

clang-format -i src/*
cmake-format --line-width 200 -i CMakeLists.txt
