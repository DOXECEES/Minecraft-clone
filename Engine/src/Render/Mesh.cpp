// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Mesh.hpp"

Renderer::Mesh::Mesh(std::vector<GLfloat> &vertices, std::vector<unsigned int> &indices)
{
    this->vertices = vertices;
    this->indices = indices;

    vao = new Renderer::VertexArray();
    vao->Link(vertices, indices);
}

void Renderer::Mesh::Draw(Shader &shader)
{
    shader.Use();
    vao->Bind();
    vao->Draw(indices.size());
    vao->UnBind();
}

Renderer::Mesh::~Mesh()
{
    delete vao;
}