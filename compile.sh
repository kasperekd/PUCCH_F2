#!/bin/bash

if ! command -v cmake &> /dev/null
then
    echo "CMake could not be found."
    exit 1
fi

mkdir -p build
cd build
cmake ..
make

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

echo "Compilation successful."