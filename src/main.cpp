#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "program.h"

int main() {
    Program program = Program();
    while (!glfwWindowShouldClose(program.window)) {
      program.update();
      program.render();
    }

    return 0;
}
