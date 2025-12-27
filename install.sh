#!/usr/bin/env bash

dirname="ctask"

echo 'Cloning git repository...'
eval git clone "https://github.com/Amir-jpg-png/$dirname.git"

echo "Moving into source directory..."
eval cd ctask || exit 1

echo "Building makefiles into build..."
eval cmake -S . -B build

echo "Moving into build directory"
eval cd build || exit 1

echo "Compiling application..."
eval make

echo "The following will require sudo permissions. "

eval sudo mv ../bin/ctask /usr/local/bin/

echo "The script was successfully installed. "

echo "Removing leftover artifacts..."
eval cd ../../ && rm -rf ctask || exit 1
