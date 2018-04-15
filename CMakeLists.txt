cmake_minimum_required (VERSION 2.6)
project (Evolve)

# The version number.
set (Evolve_VERSION_MAJOR 1)
set (Evolve_VERSION_MINOR 2)

# Enable Debugging
set(CMAKE_BUILD_TYPE Debug)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/EvolveConfig.h.in"
  "${PROJECT_BINARY_DIR}/include/EvolveConfig.h"
  )
 
# Add the include directory that contains Cmake settings
include_directories(build/include)

# Add headers from the source code
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

# add the executeable
 add_executable(Evolve ${SOURCES})