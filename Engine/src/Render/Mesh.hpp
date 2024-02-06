#pragma once

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace Renderer
{
    class Mesh
    {
    public:
        Mesh(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
        ~Mesh();
        void Draw(Shader &shader);

    private:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;
        Renderer::VertexArray *vao;
    };
};