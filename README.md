# Deribit-Test-Trader

A C++ application for automated trading on the Deribit cryptocurrency exchange's testnet, using WebSocket API for real-time communication.

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

## Required libraries (automatically installed via vcpkg)

- Boost
- OpenSSL
- WebSocket++
- nlohmann-json

## Setup

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

    When running for the first time, run using:

    ```
    .\setup_and_run.ps1
    ```

    Or for subsequent runs:

    ```
    .\run.ps1
    ```

## Project Structure

- include/ - Header files
- src/ - Source files
- test/ - Benchmark tests
- main.cpp - Main application entry point
- CMakeLists.txt - CMake build configuration
- setup_and_run.ps1 - Initial setup and run script
- run.ps1 - Build and run script for subsequent uses

## Additional Details

To [test](https://github.com/HimanS1708/Deribit-Test-Trader/tree/main/tests) the project you can use a pre-built script which benchmarks each functionality.
