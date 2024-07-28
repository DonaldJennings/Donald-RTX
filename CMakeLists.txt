# Write CMake file to configure the project

# Set the minimum version of CMake that can be used
cmake_minimum_required(VERSION 3.0)

# Set the project name
project(DonaldRTX)

# Set the C++ standard to use
set(CMAKE_CXX_STANDARD 17)

# GLOB command finds all source files that match the specified patterns
file(GLOB SOURCES "source/*.cpp" "source/*.h")


# Add an executable target called DonaldRTX
add_executable(DonaldRTX ${SOURCES})
