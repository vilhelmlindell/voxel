#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <stb/stb_image.h>

#include "program.h"

int main() {
    Program program = Program();
    float delta_time = 0.0f; // Time between current frame and last frame
    float last_frame = 0.0f; // Time of last frame
                             
    while (!glfwWindowShouldClose(program.window)) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        program.update(delta_time);
        program.render();

        glfwSwapBuffers(program.window);
        glfwPollEvents();
    }

    return 0;
}
