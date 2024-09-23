#pragma once

#include "camera.h"
#include "shader.h"
#include "chunk_pool.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>

namespace fs = std::filesystem;


// NOTE: This assumes that the binary is run from the build folder
const fs::path WORKING_PATH = fs::current_path().parent_path();

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

class Program {
  public:
    GLFWwindow* window;

    Program();
    ~Program();

    void update(float delta_time);
    void render();
    void cleanup();

  private:
    std::unique_ptr<ChunkPool> world;
    Camera camera;
    bool show_demo_window = true;
    bool first_mouse = true;
    glm::vec2 last_mouse_pos;

    GLFWwindow* init_window();
    void init_imgui();
    void init_buffers();
    void process_input();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};
