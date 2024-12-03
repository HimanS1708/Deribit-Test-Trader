cmake_minimum_required(VERSION 3.31.1)

# Set vcpkg toolchain file (keep this before project() command)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" 
    CACHE STRING "Vcpkg toolchain file")

project(DeribitTestTrader VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Boost REQUIRED COMPONENTS thread)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(websocketpp CONFIG REQUIRED)

# Add source files
set(SOURCES
    main.cpp
    src/connection.cpp
    src/deribitClient.cpp
    src/token.cpp
)

# Add your executable (replace main.cpp with your source files)
add_executable(${PROJECT_NAME} ${SOURCES})

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Specify this is a console application
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE FALSE)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Boost::boost
    Boost::system
    Boost::thread
    OpenSSL::SSL
    OpenSSL::Crypto
    ws2_32 
    nlohmann_json::nlohmann_json 
    websocketpp::websocketpp
)
