// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdint.h>
#include <ft2build.h>
#include <glm.hpp>
#include FT_FREETYPE_H

#include <map>

#include "../../Logger.hpp"

namespace TextRenderer
{
    struct Character
    {
        GLuint TextureID;   // ID текстуры глифа
        glm::ivec2 Size;    // Размеры глифа
        glm::ivec2 Bearing; // Смещение верхней левой точки глифа
        GLuint Advance;     // Горизонтальное смещение до начала следующего глифа
    };

    class Text
    {
    public:
        Text();
        ~Text() = default;

    private:
        std::map<char, Character> characters;
    };

};