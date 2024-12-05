# Deribit Test Trader Documentation

## Overview
Deribit Test Trader is a C++ application that interfaces with the Deribit cryptocurrency exchange API through WebSocket connections. It provides functionality for trading, order management, and market data streaming on the Deribit test network.

## Features

- Real-time WebSocket connection to Deribit testnet
- Authentication and automatic token refresh
- Place, modify and cancel orders
- View order book data
- Monitor current positions
- Subscribe to and stream real-time price index updates
- Clean command-line interface

## Pre-requisites

- CMake (3.31.1 or higher)
- MinGW-w64 with GCC (13 or higher)
- Visual Studio 2022 or higher
- PowerShell

## Build and Setup

1. Clone the repository:

    ```
    git clone "https://github.com/HimanS1708/Deribit-Test-Trader.git"
    cd Deribit-Test-Trader
    ```

2. Create a file for API keys named api_keys.json.

3. Configure your API keys: 

    In the file *api_keys.json*, update the fields.
    ```json
    {
        "clientId": "your_client_id",
        "clientSecret": "your_client_secret"
    }
    ```

4. Build and run:

    This project uses CMake for build management and vcpkg for dependency management. When running for the first time, run using:

    ```
    .\setup_and_run.ps1
    ```

    Or for subsequent runs:

    ```
    .\run.ps1
    ```

## Project Structure

**NOTE**: This is from the root directory of the project.

- include/ - Header files
- src/ - Source files
- test/ - Benchmark tests
- main.cpp - Main application entry point
- CMakeLists.txt - CMake build configuration
- setup_and_run.ps1 - Initial setup and run script
- run.ps1 - Build and run script for subsequent uses

## API Integration

The application integrates with Deribit's WebSocket API v2, supporting:

- Real-time order management
- Market data streaming
- Position monitoring
- Authentication management

Keys are stored in `api_keys.json` and should be configured before running the application.

## Thread Management

The application utilizes multi-threading to handle different aspects of trading operations efficiently:

### 1. Connection Thread
- Manages the WebSocket connection to Deribit's servers
- Runs continuously in the background via `connectionThread` in `main.cpp`
- Handles connection establishment, maintenance, and message processing

### 2. Authentication Thread
- Manages token renewal and authentication state
- Runs on a separate thread (`authThread` in `main.cpp`)
- Periodically checks token expiration (every 5 seconds)
- Automatically refreshes authentication when needed

### 3. Main Thread and Synchronization
- Main Thread manages the main logic of the application
- Uses `std::thread` for thread management
- Implements thread joining at application shutdown

## Latency Management

The application implements several strategies to minimize latency:

### Connection Optimization
- Direct WebSocket connection to reduce overhead
- Minimal logging configuration to reduce I/O overhead

### Message Processing
- Asynchronous message handling
- Non-blocking operations for order management and message handling/logging

## Benchmarking

Performance testing is implemented in `tests/speed_test.cpp` using a custom Timer class:

### Metrics Measured
- Order place, modify and cancel time
- Fetching order book time
- Viewing current position time
- Message round-trip time

### Timer Implementation
```cpp
Timer(){
    m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

~Timer(){
    auto m_endTimepoint = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_endTimepoint).time_since_epoch().count();
    auto duration = end - start;
    double ms = duration * 0.001;
    std::cout << duration << "us (" << ms << "ms)\n\n";
}
```

Wherever this Timer 

## Future Improvements

### 1. Performance Enhancements
- Implement connection pooling for multiple concurrent operations
- Add message queuing system for high-frequency trading
- Implement batch order processing

### 2. Reliability Improvements
- Enhance error logging and monitoring
- Add automatic reconnection mechanism if connection fails

### 3. Feature Additions
- Add support for real-time market analysis
- Implement trading strategies framework
- Include all trading parameters which were now excluded

### 4. Testing Enhancements
- Add comprehensive unit tests
- Implement integration testing suite
- Add stress testing capabilities
- Create automated performance benchmarking

## Demonstration Video

The video can be found [here](https://youtu.be/Llt2qbUCxdE).
