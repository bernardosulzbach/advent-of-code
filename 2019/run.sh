#!/usr/bin/env bash
clang-format -i ../src/*
make -j 12

./01 ../input/01.txt >../output/01.txt

./02 ../input/02.txt >../output/02.txt

for i in {1..3}; do
  ./03 ../samples/03-$i.txt >../output/samples/03-$i.txt
done

./03 ../input/03.txt >../output/03.txt
