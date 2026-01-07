#!/bin/bash
set -e

echo "Building System Monitor..."
mkdir -p logs build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

echo "Starting monitoring..."
./src/SystemMonitor
