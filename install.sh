#!/usr/bin/env bash

dirname="ctask"

echo 'Cloning git repository...'
git clone "https://github.com/Amir-jpg-png/$dirname.git"

echo "Moving into source directory..."
cd ctask || exit 1

echo "Building makefiles into build..."
cmake -S . -B build

echo "Moving into build directory"
cd build || exit 1

echo "Compiling application..."
make

echo "The following will require sudo permissions. "

eval sudo mv ../bin/ctask /usr/local/bin/

echo "The script was successfully installed. "
