# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PongClone VERSION 1.0)

# Set C standard to C89
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED True)

# Enable FetchContent module
include(FetchContent)

# Download and configure SDL2
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.28.4  # Use the desired version tag or branch
)

FetchContent_MakeAvailable(SDL2)

# Add the executable file
add_executable(PongClone src/main.c)

# Include SDL2 headers and link SDL2 library
target_link_libraries(PongClone PRIVATE SDL2::SDL2 SDL2::SDL2main)

# Set output directories (optional)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

