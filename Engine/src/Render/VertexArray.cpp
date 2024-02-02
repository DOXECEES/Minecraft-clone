
#include "VertexArray.hpp"

Renderer::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &VAO);
}

void Renderer::VertexArray::Bind()
{
    glBindVertexArray(VAO);
}

void Renderer::VertexArray::UnBind()
{
    glBindVertexArray(0);
}

void Renderer::VertexArray::Link(const std::vector<GLfloat> &data, const std::vector<GLuint> &indices)
{
    Bind();
    std::unique_ptr<VertexBuffer> VBO(new VertexBuffer());
    VBO->Bind();
    VBO->SetData(data);

    std::unique_ptr<IndexBuffer> EBO(new IndexBuffer());
    EBO->Bind();
    EBO->SetElements(indices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

    VertexBuffers.push_back(VBO->GetVBO());
    IndexBuffers.push_back(EBO->GetEBO());
    glBindVertexArray(0);
}

void Renderer::VertexArray::Draw(GLsizei count)
{
    for (size_t i = 0; i < VertexBuffers.size() + 1; i++)
    {
        glEnableVertexAttribArray(i);
    }

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

    for (size_t i = 0; i < VertexBuffers.size() + 1; i++)
    {
        glDisableVertexAttribArray(i);
    }
}

Renderer::VertexArray::~VertexArray()
{
    glDeleteBuffers(VertexBuffers.size(), VertexBuffers.data());
    glDeleteBuffers(IndexBuffers.size(), IndexBuffers.data());
    glDeleteVertexArrays(1, &VAO);
}
