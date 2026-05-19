#pragma once

#include <cstdint>
#include <string>

#include "glad/glad.h"

enum class TextureType : std::uint8_t
{
    Diffuse,
    Normal,
    Specular,
    Roughness,
    Metallic,
    AO
};

inline auto toString(const TextureType type) -> std::string
{
    switch (type)
    {
    case TextureType::Diffuse:
        return "diffuseMap";
    case TextureType::Normal:
        return "Normal";
    case TextureType::Specular:
        return "Specular";
    case TextureType::Roughness:
        return "Roughness";
    case TextureType::Metallic:
        return "Metallic";
    case TextureType::AO:
        return "AO";
    default:
        return "Unknown";
    }
}

class Texture
{
  public:
    explicit Texture(const std::string &path, TextureType type, bool flip = false);
    explicit Texture(int width, int height);

    ~Texture();

    Texture(const Texture &) = delete;
    auto operator=(const Texture &) -> Texture & = delete;

    Texture(Texture &&other) noexcept : id(other.id), type(other.type)
    {
        other.id = 0;
    }

    auto operator=(Texture &&other) noexcept -> Texture &
    {
        if (this != &other)
        {
            glDeleteTextures(1, &id);
            id = other.id;
            type = other.type;
            other.id = 0;
        }
        return *this;
    }

    void bind(GLuint unit = 0) const;
    [[nodiscard]] auto getId() const -> GLuint
    {
        return id;
    }
    [[nodiscard]] auto getType() const -> TextureType
    {
        return type;
    }

  private:
    GLuint id = 0;
    TextureType type;
};
