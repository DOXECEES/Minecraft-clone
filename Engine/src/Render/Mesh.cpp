
#include "Mesh.hpp"

Renderer::Mesh::Mesh(std::vector<GLfloat> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> &textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    vao = new Renderer::VertexArray();
    vao->Link(vertices, indices);
}

void Renderer::Mesh::setupMesh()
{
    vao = new Renderer::VertexArray();
    vao->Link(vertices, indices);
    // textures[0]
}

void Renderer::Mesh::Draw(Shader &shader)
{
    // glActiveTexture(GL_TEXTURE0);
    textures[0].Bind();
    vao->Bind();
    vao->Draw(indices.size());
    vao->UnBind();
    textures[0].UnBind();
}

Renderer::Mesh::~Mesh()
{
    delete vao;
}