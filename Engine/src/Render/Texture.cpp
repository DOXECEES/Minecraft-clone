
#include "Texture.hpp"

Renderer::Texture::Texture()
{
    glGenTextures(1, &texture);
}

void Renderer::Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Renderer::Texture::UnBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::Texture::SetData(const std::vector<unsigned char> &data, const GLuint width, const GLuint height)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    UnBind();
}

void Renderer::Texture::LoadTexture(const std::string &path)
{
    std::vector<unsigned char> data;

    if (lodepng::decode(data, width, height, path))
    {
        Logger::Log("Failed to load texture on path: " + path, Logger::ERROR);
    }

    SetData(data, width, height);
}

float Renderer::Texture::GetSquare() const
{
    return static_cast<float>(width * height);
}

Renderer::Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}