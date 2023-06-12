
#include "Shader.hpp"


Renderer::Shader::Shader()
{
    
}


bool Renderer::Shader::LoadShader(std::string path)
{
    std::ifstream shader;
    shader.open(path);

    if(!shader.is_open())
    {
        std::cout << 
    }

    std::stringstream ss;
    shader >> ss;



}