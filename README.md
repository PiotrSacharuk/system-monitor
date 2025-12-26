# start
docker build -t system-monitor .
docker run -it --rm -v $(pwd):/app system-monitor

# inside container
cd build && make clean && cmake .. && make -j$(nproc) && ./SystemMonitor