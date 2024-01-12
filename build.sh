#!/bin/bash

# Update git submodules
git submodule update --init --recursive

# Check if cmake is installed
if ! command -v cmake &> /dev/null
then
    echo "CMake could not be found. Please install CMake to continue."
    exit 1
fi

cd src/assets/textures
python stitch.py
python header.py

cd ../../..

# Create and navigate to the build directory
mkdir -p build
cd build

# Build the project
cmake ..
cmake --build . --config Release


# Move and execute the app
./app