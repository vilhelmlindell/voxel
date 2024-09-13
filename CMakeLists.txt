cmake_minimum_required(VERSION 3.26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable debug symbols
set(CMAKE_BUILD_TYPE Debug)

# Enable both C and C++ languages in the project
project(voxel
  VERSION 1.0
  LANGUAGES C CXX)

# Source files for the voxel executable
set(SOURCE 
  src/main.cpp
  src/shader.cpp
)

# Libraries to link against
set(LIBRARY
  glad
  glfw
  OpenGL::GL
  glm::glm
)

# Find required packages
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# Add glad as a C static library and set include directory for glad
add_library(glad STATIC src/glad.c)

# Make sure glad can find its header file
target_include_directories(glad
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include  # Add the include directory where glad.h is located
)

# Add executable
add_executable(voxel "${SOURCE}")

# Link libraries
target_link_libraries(voxel "${LIBRARY}")

# Specify C++ standard
set_target_properties(voxel PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

# Add include directories for voxel executable
target_include_directories(voxel
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include  # Local include directory
)

# Install target
install(TARGETS voxel DESTINATION bin)
