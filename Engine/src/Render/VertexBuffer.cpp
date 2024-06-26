// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "VertexBuffer.hpp"

Renderer::VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &VBO);
}

void Renderer::VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void Renderer::VertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::VertexBuffer::SetData(const std::vector<GLfloat> &data)
{
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
}

GLuint Renderer::VertexBuffer::GetVBO()
{
    return VBO;
}
