
#include "VertexArray.hpp"

Renderer::VertexArray::VertexArray()
{
    glGenVertexArrays(1,&VAO);
}


void Renderer::VertexArray::Bind()
{
    glBindVertexArray(VAO);
}


void Renderer::VertexArray::Link(const std::vector<GLfloat> &data)
{
    std::unique_ptr<VertexBuffer> VBO(new VertexBuffer());

    VBO->Bind();
    VBO->SetData(data);
    

    glVertexAttribPointer(VertexBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    VertexBuffers.push_back(VBO->GetVBO());
}


void Renderer::VertexArray::Draw(GLsizei count)
{
 
    for(size_t i = 0; i < VertexBuffers.size(); i++)
    {
        glEnableVertexAttribArray(i);    
    }

    glDrawArrays(GL_TRIANGLES, 0, count);

    for(size_t i = 0; i < VertexBuffers.size(); i++)
    {
        glDisableVertexAttribArray(i);
    }
}


Renderer::VertexArray::~VertexArray()
{
    
    glDeleteBuffers(VertexBuffers.size(),VertexBuffers.data());
    glDeleteVertexArrays(1,&VAO);
}


