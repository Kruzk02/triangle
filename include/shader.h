#pragma once

#include <glad.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Shader {
    mutable std::unordered_map<std::string, int> uniformCache;

    int getUniformLocation(const std::string &name) const {
        if (uniformCache.find(name) != uniformCache.end()) {
            return uniformCache[name];
        }

        int location = glGetUniformLocation(ID, name.c_str());
        uniformCache[name] = location;
        return location;
    }

public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath) {
        std::ifstream vFile(vertexPath);
        if (!vFile) {
            std::cerr << "ERROR::SHADER::VERTEX FILE NOT FOUND\n";
            return;
        }
        std::string vertexCode((std::istreambuf_iterator<char>(vFile)), std::istreambuf_iterator<char>());

        std::ifstream fFile(fragmentPath);
        if (!fFile) {
            std::cerr << "ERROR::SHADER::FRAGMENT FILE NOT FOUND\n";
            return;
        }
        std::string fragmentCode((std::istreambuf_iterator<char>(fFile)), std::istreambuf_iterator<char>());

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileError(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileError(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileError(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(getUniformLocation(name), (int) value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(getUniformLocation(name), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(getUniformLocation(name), value);
    }

private:
    void checkCompileError(unsigned int shader, std::string type) const {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
}; 