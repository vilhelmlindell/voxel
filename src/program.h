#pragma once

#include "camera.h"
#include "shader.h"
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "world.h"

namespace fs = std::filesystem;

// NOTE: This assumes that the binary is run from the build folder
const fs::path WORKING_PATH = fs::current_path().parent_path();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Program {
public:
  GLFWwindow *window;

  Program();
  ~Program();

  void update();
  void render();
  void cleanup();

private:
  World<10, 10, 10> world;
  Camera camera;
  std::unique_ptr<Shader> shader;
  unsigned int vbo, vao, ebo, texture;
  bool show_demo_window = true;
  bool first_mouse = true;
  glm::vec2 mouse_pos, last_mouse_pos;

  GLFWwindow* initialize_window();
  void initialize_imgui();
  void initialize_buffers();
  void process_input();

  void mouse_callback(GLFWwindow* window, double xpos, double ypos);
  void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};
