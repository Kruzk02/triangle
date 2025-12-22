#pragma once

#include <glad/glad.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
    mutable std::unordered_map<std::string, int> uniformCache;

    int getUniformLocation(const std::string &name) const {
        if (uniformCache.find(name) != uniformCache.end()) {
            return uniformCache[name];
        }

        const int location = glGetUniformLocation(ID, name.c_str());
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
        std::string vertexCode((std::istreambuf_iterator(vFile)), std::istreambuf_iterator<char>());

        std::ifstream fFile(fragmentPath);
        if (!fFile) {
            std::cerr << "ERROR::SHADER::FRAGMENT FILE NOT FOUND\n";
            return;
        }
        std::string fragmentCode((std::istreambuf_iterator(fFile)), std::istreambuf_iterator<char>());

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileError(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
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

    void use() const {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, const bool value) const {
        glUniform1i(getUniformLocation(name), static_cast<int>(value));
    }

    void setInt(const std::string &name, const int value) const {
        glUniform1i(getUniformLocation(name), value);
    }

    void setFloat(const std::string &name, const float value) const {
        glUniform1f(getUniformLocation(name), value);
    }

    void setMat4(const std::string &name, const glm::mat4 &value) const {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    static void checkCompileError(const unsigned int shader, const std::string& type) {
        int success;
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                int logLength;
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
                std::vector<char> infoLog(logLength);
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog.data());
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog.data() << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                int logLength;
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
                std::vector<char> infoLog(logLength);
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog.data());
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog.data() << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
}; 