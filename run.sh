#!/bin/bash
set -e

echo "Building System Monitor..."
mkdir -p logs

PRESET_NAME=release
cmake --preset=${PRESET_NAME} .
cd build/${PRESET_NAME}
make -j$(nproc)
cd ../..

echo "Starting monitoring..."
./build/${PRESET_NAME}/src/SystemMonitor
