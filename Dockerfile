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
RUN cmake -S . -B build-release --preset=release  && \
    cmake --build build-release -j$(nproc) && \
    cmake --install build-release --prefix /install

# Test/Coverage stage
FROM base AS test-cov
RUN cmake -S . -B build-cov --preset=ci-debug && \
    cmake --build build-cov -j$(nproc) && \
    cmake --install build-cov --prefix /install

CMD ["bash", "-c", "\
    ctest --test-dir build-cov --output-on-failure && \
    mkdir -p coverage && \
    gcovr -r . /install/bin --html-details coverage/index.html --xml coverage/coverage.xml \
    "]

# Production stage
FROM ubuntu:24.04 AS prod
WORKDIR /app

RUN apt-get update && apt-get install -y \
    libyaml-cpp0.8 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=dev /install/bin/SystemMonitor ./SystemMonitor

VOLUME ["/app/logs"]
CMD ["./SystemMonitor"]