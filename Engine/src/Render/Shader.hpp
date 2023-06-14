#pragma once

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include "../Logger.hpp"

namespace Renderer
{
    class Shader
    {
        public:

            Shader();
            ~Shader() = default;

            bool LoadShader(const std::string& path);
            void  CreateProgram();
            GLuint GetProgram() { return shaderProgram; };

        private:
            void CompileShader(GLuint &shader);

        private:
            GLuint shaderProgram;
            GLuint vertexShader;
            GLuint fragmentShader;
    };


};
