# System Monitor

A comprehensive C++ system monitoring application that tracks CPU, RAM, and disk usage with configurable thresholds and logging capabilities.

## Features

- **Real-time Monitoring**: Continuous monitoring of system resources
- **Multi-sensor Support**: CPU, RAM, and Disk sensors with customizable thresholds
- **Configurable**: YAML-based configuration for easy setup
- **Logging**: Built-in logging system with configurable levels
- **Statistics**: Tracks averages, maximums, and trends for each sensor
- **Test/Production Modes**: Supports both testing cycles and infinite production monitoring
- **Docker Support**: Containerized deployment with multi-stage builds
- **Comprehensive Testing**: Unit tests with GoogleTest and coverage reporting
- **Cross-platform**: Built with CMake for Linux/Windows compatibility

## Prerequisites

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.25 or higher
- yaml-cpp library (automatically fetched if not found)
- GoogleTest (automatically fetched for testing)

## Quick Start

### Using the Run Script

```bash
# Clone and run
git clone https://github.com/PiotrSacharuk/system-monitor
cd system-monitor
chmod +x run.sh
./run.sh
```

### Manual Build

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build
make -j$(nproc)

# Run
./src/system-monitor
```

## Configuration

Copy `config.example.yaml` to `config.yaml` and modify as needed:

```yaml
server:
  name: "Prod-Server-01"

monitoring:
  interval_seconds: 5
  test_cycles: 12    # -1 = infinite (production)

sensors:
  cpu:
    threshold: 85
    cores: 8
  ram:
    threshold: 90
  disk:
    threshold: 90

logging:
  enabled: true
  level: "INFO"
```

### Configuration Options

- **server.name**: Identifier for the monitored server
- **monitoring.interval_seconds**: Seconds between monitoring cycles
- **monitoring.test_cycles**: Number of test cycles (-1 for infinite/production mode)
- **sensors.cpu.threshold**: CPU usage alert threshold (percentage)
- **sensors.cpu.cores**: Number of CPU cores to monitor
- **sensors.ram.threshold**: RAM usage alert threshold (percentage)
- **sensors.disk.threshold**: Disk usage alert threshold (percentage)
- **logging.enabled**: Enable/disable logging
- **logging.level**: Log level (DEBUG, INFO, WARN, ERROR)

## Docker Usage

### Development Build

```bash
docker build -t system-monitor:dev --target dev .
docker run -it --rm -v $(pwd)/logs:/app/logs system-monitor:dev
```

### Testing with Coverage

```bash
docker build -t system-monitor:test --target test-cov .
docker run -it --rm -v $(pwd)/coverage:/app/coverage system-monitor:test
```

### Production Build

```bash
docker build -t system-monitor:prod --target prod .
docker run -it --rm -v $(pwd)/logs:/app/logs system-monitor:prod
```

## Testing

### Run Tests

```bash
cd build
ctest -V
```

## Build Presets

The project includes several CMake presets for different build configurations:

- **release**: Optimized production build
- **ci-debug**: Debug build with coverage instrumentation
- **asan**: AddressSanitizer enabled for memory debugging

```bash
# Using presets
cmake --preset=release
cmake --build build/release
```

## Project Structure

```
system-monitor/
├── src/                         # Source code
│   ├── main.cpp                 # Application entry point
│   ├── core/                    # Core components
│   │   ├── config.h/cpp         # Configuration management
│   │   ├── logger.h/cpp         # Logging system
│   │   └── system-monitor.h/cpp # Main monitoring logic
│   └── sensors/                 # Sensor implementations
│       ├── sensor.h             # Base sensor interface
│       ├── cpu_sensor.h/cpp
│       ├── ram_sensor.h/cpp
│       └── disk_sensor.h/cpp
├── tests/                      # Unit tests
├── config.example.yaml         # Configuration template
├── CMakeLists.txt              # CMake configuration
├── CMakePresets.json           # Build presets
├── Dockerfile                  # Docker configuration
├── docker-compose.prod.yml     # Production compose
└── run.sh                      # Quick start script
```

## Architecture

The application follows a modular sensor-based architecture:

- **SystemMonitor**: Central coordinator that manages sensors and collects data
- **Sensors**: Individual components for monitoring specific system resources
- **Config**: Singleton configuration manager using YAML
- **Logger**: Centralized logging with configurable output
