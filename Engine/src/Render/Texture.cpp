
#include "Texture.hpp"


Renderer::Texture::Texture()
{
    glGenBuffers(1,&texture);
}


void Renderer::Texture::Bind()
{
    glBindBuffer(GL_TEXTURE_2D,texture);
}


Renderer::Texture::~Texture()
{
    
}