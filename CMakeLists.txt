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

set(IMGUI_SOURCE 
  include/imgui/imgui.cpp
  include/imgui/imgui_demo.cpp
  include/imgui/imgui_draw.cpp
  include/imgui/imgui_tables.cpp
  include/imgui/imgui_widgets.cpp
  include/imgui/backends/imgui_impl_glfw.cpp
  include/imgui/backends/imgui_impl_opengl3.cpp
  include/imgui/misc/cpp/imgui_stdlib.cpp
)

# Libraries to link against
set(LIBRARY
  glad
  imgui
  glfw
  OpenGL::GL
  glm::glm
)

# Find required packages
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# Add glad as a C static library and set include directory for glad
add_library(glad STATIC include/glad/glad.c)
add_library(imgui STATIC "${IMGUI_SOURCE}")

target_include_directories(imgui
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include/imgui 
)

target_include_directories(glad
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include
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
  ${CMAKE_SOURCE_DIR}/include/imgui  # Local include directory
)

# Install target
install(TARGETS voxel DESTINATION bin)
