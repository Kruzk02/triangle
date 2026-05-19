#pragma once

#include "glad/glad.h"
#include "texture.hpp"

class FrameBuffer

{
  public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void bind() const;
    void unbind() const;

    void resize(int width, int height);

    auto getColorAttachment() -> Texture &;

  private:
    GLuint fbo = 0;
    GLuint rbo = 0;

    Texture colorAttachment;

    int width = 0;
    int height = 0;
};
