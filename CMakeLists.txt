cmake_minimum_required(VERSION 3.10)
project(raytracing)
add_executable(raytracing src/main.cpp)

# Add all source files under the 'src' directory recursively
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.c")

# Add the source files to the target
target_sources(raytracing PRIVATE ${SOURCE_FILES})