#include "texture.hpp"
#include "stb_image.h"

#include <stdexcept>

Texture::Texture(const std::string &path, const TextureType type, bool flip) : type(type)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_set_flip_vertically_on_load(flip);

    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        throw std::runtime_error("failed to load texture: " + path);
    }

    GLint format;
    switch (channels)
    {
    case 1:
        format = GL_RED;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        throw std::runtime_error("Unsupported channel count: " + std::to_string(channels));
    }

    GLint internalFormat = format;
    if (format == GL_RGB)
        internalFormat = GL_SRGB;
    if (format == GL_RGBA)
        internalFormat = GL_SRGB_ALPHA;

    glBindTexture(GL_TEXTURE_2D, id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE,
                 data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

Texture::Texture(int width, int height)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
    }
}

void Texture::bind(const GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}
