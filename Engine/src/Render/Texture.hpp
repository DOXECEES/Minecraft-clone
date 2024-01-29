#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../dependencies/lodepng/lodepng.h"
#include "../Logger.hpp"

#include <string>
#include <vector>

namespace Renderer
{

    class Texture
    {
    public:
        Texture();
        ~Texture();

        void Bind();
        void UnBind();
        void SetData(const std::vector<unsigned char> &data, const GLuint width, const GLuint height);
        void LoadTexture(const std::string &path);

    private:
        GLuint texture;
        uint32_t width;
        uint32_t height;
    };

};
