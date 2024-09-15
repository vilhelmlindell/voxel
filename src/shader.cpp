#include "shader.h"
#include <glad/glad.h>

std::string readFile(const fs::path& path) {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(path.c_str());
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    } catch (const std::ifstream::failure &e) {
        throw std::runtime_error(
            "Could not read file at path: " + std::string(path) +
            " because of error: " + e.what());
    }
}

Shader::Shader(const fs::path& vertexPath, const fs::path& fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexSource = vertexCode.c_str();
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentSource = fragmentCode.c_str();
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    // compile shader Program
    ID = glCreateProgram();

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    check_compile_errors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
};

void Shader::check_compile_errors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Shader compilation error of type: " + type + "\n-----------\nInfo\n" +
                             infoLog
                      << "-----------" << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Program linking error of type: " + type + "\n" +
                             infoLog
                      << "-----------" << std::endl;
        }
    }
}

void Shader::use() { glUseProgram(ID); }
void Shader::set_bool(std::string_view name, bool value) {
    glUniform1i(glGetUniformLocation(ID, name.data()), (int)value);
}
void Shader::set_int(std::string_view name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.data()), value);
}
void Shader::set_float(std::string_view name, float value) {
    glUniform1i(glGetUniformLocation(ID, name.data()), value);
}
