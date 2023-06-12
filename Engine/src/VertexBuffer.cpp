
#include "VertexBuffer.hpp"


Render::VertexBuffer::VertexBuffer()
{
    glGenBuffers(0,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
}


Render::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(0,&VBO);
}



