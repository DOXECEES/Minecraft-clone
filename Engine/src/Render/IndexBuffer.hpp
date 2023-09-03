
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>


namespace Renderer
{
    class IndexBuffer
    {
    public:

        IndexBuffer();
        IndexBuffer(const IndexBuffer& other) = default;
        IndexBuffer& operator=(const IndexBuffer&) = default;

        ~IndexBuffer() = default;

        void Bind();
        void SetElements(const std::vector<GLuint>& indices);

    private:
        GLuint EBO;

    };
};