#pragma once

#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

// NOTE: This assumes that the binary is run from the build folder
const std::string WORKING_PATH =
    std::filesystem::current_path().parent_path().string();
const std::string ASSET_PATH = WORKING_PATH + "/assets/";

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Program {
public:
  Program();
  ~Program();

  void update();
  void render();
  void cleanup();

private:
  GLFWwindow *window;

  GLFWwindow* initialize_window();
  void initialize_imgui();
  void initialize_buffers();
  void load_texture();
  void process_input();
};
