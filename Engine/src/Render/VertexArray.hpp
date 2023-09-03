#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace Renderer
{
    class VertexArray
    {
        public:

            VertexArray();
            VertexArray(const VertexArray& other) = delete;
            VertexArray& operator=(const VertexArray&) = delete;
            ~VertexArray();

            void Bind();
            void Draw(GLsizei count);
            void Link(const std::vector<GLfloat> &data, const std::vector<GLuint> &indices);

        private:
            GLuint VAO;

            std::vector<GLuint> VertexBuffers;
            std::vector<GLuint> IndexBuffers;


    };

}