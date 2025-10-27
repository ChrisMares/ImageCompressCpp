# Set the target system
set(CMAKE_SYSTEM_NAME Windows)

# Specify the cross-compilers
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Specify the resource compiler
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Set the root for MinGW headers and libraries
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Configure find* commands
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
