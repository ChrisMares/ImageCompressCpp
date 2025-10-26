cmake_minimum_required(VERSION 3.10)
project(ImageCompressCpp VERSION 1.0)

# Set C++ standard (using 17, as it's common and well-supported)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define where the compiled executables should be placed
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Define the executable target
add_executable(ImageCompressCpp
    src/main.cpp
    src/file_helpers.cpp
)

# Tell CMake where to find all your headers
target_include_directories(ImageCompressCpp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include   # For file_helpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}           # For stb_image.h, etc.
)

# Link the math library (needed for stb on Linux)
if (UNIX)
    target_link_libraries(ImageCompressCpp PRIVATE m)
endif()