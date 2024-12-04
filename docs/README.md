# Deribit Test Trader Documentation

## Overview
Deribit Test Trader is a C++ application that interfaces with the Deribit cryptocurrency exchange API through WebSocket connections. It provides functionality for trading, order management, and market data streaming on the Deribit test network.

## Build and Setup

The project uses CMake for build management and vcpkg for dependency management. Two PowerShell scripts are provided:

- `setup_and_run.ps1`: First-time setup and run
- `run.ps1`: Subsequent builds and runs

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
