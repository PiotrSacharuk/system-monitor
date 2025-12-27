# start
```
docker build -t system-monitor .
docker run -it --rm -v $(pwd):/app system-monitor
```

# inside container
```
mkdir build
cd build && make clean && cmake .. && make -j$(nproc) && ./SystemMonitor
```

# testing
```
ctest -V
```

# coverage
```
cd build
make clean && cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
ctest
make coverage
```