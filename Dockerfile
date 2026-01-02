FROM ubuntu:24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    lcov \
    gcovr \
    libyaml-cpp-dev \
    clang-tidy \
    cppcheck \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

FROM base AS dev
RUN cmake -S . -B build-release \
    -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build-release -j$(nproc)

# Test/Coverage stage
FROM base AS test-cov
RUN cmake -S . -B build-cov \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="--coverage" && \
    cmake --build build-cov -j$(nproc)

CMD ["bash", "-c", "\
    ctest --test-dir build-cov --output-on-failure && \
    mkdir -p coverage && \
    gcovr \
    -r . \
    build-cov \
    --html-details coverage/index.html \
    --xml coverage/coverage.xml \
    "]

# Production stage
FROM ubuntu:24.04 AS prod
WORKDIR /app
COPY --from=dev /app/build-release/SystemMonitor ./SystemMonitor
VOLUME ["/app/logs"]
CMD ["./SystemMonitor"]