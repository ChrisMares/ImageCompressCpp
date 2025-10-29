cmake_minimum_required(VERSION 3.15)
project(ImageCompressCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include FetchContent module
include(FetchContent)

# Fetch FLTK
FetchContent_Declare(FLTK
  GIT_REPOSITORY  https://github.com/fltk/fltk
  GIT_TAG         master
  GIT_SHALLOW     TRUE
)

message(STATUS "Download and build FLTK if necessary, please wait...")
FetchContent_MakeAvailable(FLTK)
message(STATUS "Download and build FLTK - done.")

# Set the executable name based on the target OS
if(WIN32)
    set(EXE_NAME ImageCompress)
else()
    set(EXE_NAME ImageCompress)
endif()

# Use the variable for the executable name
add_executable(${EXE_NAME}
    src/main.cpp
    src/image_processor.cpp
    src/arg_helper.cpp
)

# Use the variable for the target
target_include_directories(${EXE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Find and link the thread library (pthreads) for std::thread
find_package(Threads REQUIRED)

# Link libraries
target_link_libraries(${EXE_NAME} PRIVATE
    Threads::Threads
    fltk::fltk
)

# Windows-specific static linking
if(WIN32)
    target_link_libraries(${EXE_NAME} PRIVATE
        -static-libgcc
        -static-libstdc++
        -static
    )
endif()

# Link math library only on native Linux (not MinGW)
if (UNIX AND NOT MINGW)
    target_link_libraries(${EXE_NAME} PRIVATE m)
endif()
