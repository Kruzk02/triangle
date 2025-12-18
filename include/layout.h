#pragma once
#include <vector>

#include "glad/glad.h"

struct VertexAttribute {
    GLuint index;
    GLint count;
    GLenum type;
    GLboolean normalized;
    size_t offset;
};

struct VertexLayout {
    std::vector<VertexAttribute> attributes;
    GLint stride;
};
