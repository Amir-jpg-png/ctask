#!/usr/bin/env bash

dirname="ctask"

git clone "https://github.com/Amir-jpg-png/$dirname.git"

cd ctask || exit 1

cmake -S . -B build

cd build

make

cd ../bin

./ctask add test
