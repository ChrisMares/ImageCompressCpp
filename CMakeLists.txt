cmake_minimum_required(VERSION 3.10)
project(ImageCompressCpp VERSION 1.0)

# --------------------------------------------------------------------------------
# Define where the compiled executables (runtime files) should be placed.
# This directs the final 'ImageCompressCpp' binary into the 'build' directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
# --------------------------------------------------------------------------------

# Set C++ standard to C++20 for access to modern features like Ranges and Modules
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the executable target using the source file in the src/ folder
# NOTE: The executable name is now ImageCompressCpp
add_executable(ImageCompressCpp
    src/main.cpp
    src/file_helpers.cpp
)

target_include_directories(ImageCompressCpp PRIVATE ${CMAKE_SOURCE_DIR}/include)
