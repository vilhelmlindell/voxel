#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class Shader {
  public:
    unsigned int ID;
    Shader(const fs::path& vertexPath, const fs::path& fragmentPath);
    void use();
    void set_bool(std::string_view name, bool value);
    void set_int(std::string_view name, int value);
    void set_float(std::string_view name, float value);

  private:
    void check_compile_errors(unsigned int shader, std::string type);
};
