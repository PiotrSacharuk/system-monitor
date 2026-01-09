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
RUN cmake -S . -B build-release --preset=release && \
    cmake --build build-release -j$(nproc) && \
    cmake --install build-release --prefix /install

# Test/Coverage stage
FROM base AS test-cov
RUN cmake -S . -B build-cov --preset=ci-debug && \
    cmake --build build-cov -j$(nproc)

CMD ["bash", "-c", "\
    ctest --test-dir build-cov --output-on-failure && \
    mkdir -p coverage && \
    gcovr -r . build-cov \
    --html-details coverage/index.html \
    --xml coverage/coverage.xml \
    "]

FROM base AS coverity
RUN cmake -S . -B build-cov --preset=ci-debug && \
    cmake --build build-cov -j$(nproc)

# Production stage
FROM ubuntu:24.04 AS prod

RUN apt-get update && apt-get install -y \
    libyaml-cpp0.8 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=dev /install/bin/system-monitor ./system-monitor

VOLUME ["/app/logs"]
CMD ["./system-monitor"]