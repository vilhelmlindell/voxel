#include "shader.h"
#include <glad/glad.h>

void checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Shader compilation error of type: " + type +
                             "\n" + infoLog << "-----------" << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "Program linking error of type: " + type +
                             "\n" + infoLog << "-----------" << std::endl;
        }
    }
}

void loadShader(const std::string_view &path, std::string &code,
                const std::string &shaderType) {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(path.data());
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    } catch (const std::ifstream::failure &e) {
        throw std::runtime_error("Could not read " + shaderType +
                                 " file at path " + std::string(path) + " because of error:  " +
                                 e.what());
    }
}

Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        loadShader(vertexPath, vertexCode, "Vertex Shader");
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }

    try {
        loadShader(fragmentPath, fragmentCode, "Fragment Shader");
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
};

void Shader::use() { glUseProgram(ID); }
void Shader::setBool(std::string_view name, bool value) {
    glUniform1i(glGetUniformLocation(ID, name.data()), (int)value);
}
void Shader::setInt(std::string_view name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.data()), value);
}
void Shader::setFloat(std::string_view name, float value) {
    glUniform1i(glGetUniformLocation(ID, name.data()), value);
}
