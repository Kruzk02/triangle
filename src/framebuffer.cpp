#include "framebuffer.hpp"
#include "texture.hpp"
#include <iostream>

FrameBuffer::FrameBuffer(int width, int height)
    : colorAttachment(width, height), width(width), height(height)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           colorAttachment.getId(), 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << "\n";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(int width, int height)
{
    if (width <= 0 || height <= 0)
        return;

    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, colorAttachment.getId());

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

auto FrameBuffer::getColorAttachment() -> Texture &
{
    return colorAttachment;
}
