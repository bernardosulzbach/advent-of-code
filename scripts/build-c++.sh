#!/usr/bin/env bash

DIRECTORY="${BASH_SOURCE%/*}"
if [[ ! -d "$DIRECTORY" ]]; then DIRECTORY="$PWD"; fi
. "$DIRECTORY/include.sh"

BUILD_DIRECTORY="scripted-build-gcc"

build() {
  if [[ $# -ne 1 ]]; then
    echo "build() expects a single argument."
    return 1
  fi
  cd "$1" || exit 1
  mkdir -p "$BUILD_DIRECTORY" && cd "$BUILD_DIRECTORY" || exit 1
  clang-format -i ../src/*
  cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug ..
  cmake --build . --parallel "$(nproc)"
  cd ../..
}

build "common"
build "2019"
build "2020"
