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
    void setBool(std::string_view name, bool value);
    void setInt(std::string_view name, int value);
    void setFloat(std::string_view name, float value);
  private:
    void checkCompileErrors(unsigned int shader, std::string type);
};
