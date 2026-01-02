# Dev stage
FROM ubuntu:24.04 AS dev

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    lcov \
    gcovr \
    libyaml-cpp-dev \
    googletest \
    clang-tidy \
    cppcheck \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc)


# Test/Coverage stage
FROM dev AS test-cov
RUN cd build && \
    cmake -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS_DEBUG="-fprofile-arcs -ftest-coverage" .. && \
    make -j$(nproc)

CMD ["bash", "-c", "cd build && ctest -V && make coverage || true"]

# Production stage
FROM ubuntu:24.04 AS prod
WORKDIR /app
COPY --from=dev /app/build/release/SystemMonitor ./SystemMonitor
VOLUME ["/app/logs"]
CMD ["./SystemMonitor"]