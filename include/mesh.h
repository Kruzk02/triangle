#pragma once
#include <vector>

#include "glad/glad.h"
#include "layout.h"

class Mesh {
public:
    Mesh(const std::vector<float>& vertices,
         const std::vector<unsigned int>& indices,
         const VertexLayout& layout)
    : indexCount(indices.size()) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(vertices.size()) * static_cast<GLsizeiptr>(sizeof(float)),
            vertices.data(),
            GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(indices.size()) * static_cast<GLsizeiptr>(sizeof(unsigned)),
            indices.data(),
            GL_STATIC_DRAW);

        for (const auto&[index, count, type, normalized, offset] : layout.attributes) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                count,
                type,
                normalized,
                layout.stride,
                reinterpret_cast<void*>(offset)
            );
        }

        glBindVertexArray(0);
    }

    void draw(const GLenum mode = GL_TRIANGLES) const {
        glBindVertexArray(VAO);
        glDrawElements(mode, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
private:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    size_t indexCount = 0;
};
