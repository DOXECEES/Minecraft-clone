#define GLFW_INCLUDE_NONE
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace Renderer
{

    class VertexBuffer 
    {
    public:
        VertexBuffer();
        VertexBuffer(const VertexBuffer& other) = default;
        VertexBuffer& operator=(const VertexBuffer&) = default;

        ~VertexBuffer() = default;

        void Bind();
        void SetData(const std::vector<GLfloat> &data);


        GLuint GetVBO();

    private:

        GLuint VBO;

    private:
    };

}