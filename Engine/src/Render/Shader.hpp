#define GLFW_INCLUDE_NONE
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Renderer
{
    class Shader
    {
        public:

            Shader();
            ~Shader();

            bool LoadShader(std::string path);
            
        private:
            GLuint shaderProgram;

    };


};
