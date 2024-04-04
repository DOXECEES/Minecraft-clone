// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "IndexBuffer.hpp"

Renderer::IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &EBO);
}

void Renderer::IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void Renderer::IndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::IndexBuffer::SetElements(const std::vector<GLuint> &indices)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}
