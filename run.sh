#!/bin/bash
set -e

echo "Building System Monitor..."
mkdir -p logs build
cd build
cmake --preset=release ..
make -j$(nproc)
cd ..

echo "Starting monitoring..."
./build/src/SystemMonitor
