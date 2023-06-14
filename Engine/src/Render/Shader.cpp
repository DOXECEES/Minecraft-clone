
#include "Shader.hpp"


Renderer::Shader::Shader()
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	Logger::Log("Shader..." ,Logger::INFO);

}


bool Renderer::Shader::LoadShader(const std::string& path)
{
    std::ifstream shader;
    shader.open(path);
    std::string buffer;
    std::string shaderSource;

    if(!shader.is_open())
    {
        Logger::Log("Failed to open shader file [" + path + "]" ,Logger::FATAL);
        return false;
    }


    while(getline(shader,buffer))
    {
        shaderSource.append(buffer);
        shaderSource += '\n';
    }
    std::cout << shaderSource;

    size_t pos = path.find('.');

    if(path.substr(pos+1) == "vs")                          // substr(pos) = .vs
    {                                                       // substr(pos+1) = vs
        Logger::Log("Start vertex shader compilation "+path,Logger::INFO);

        const char* glShaderSourceC = shaderSource.c_str();
        glShaderSource(vertexShader,1,&glShaderSourceC,nullptr);
        Renderer::Shader::CompileShader(vertexShader);
    }
    else if(path.substr(pos+1) == "fs")                     // substr(pos) = .fs
    {                                                       // substr(pos+1) = fs
        Logger::Log("Start fragment shader compilation "+path,Logger::INFO);

        const char* glShaderSourceC = shaderSource.c_str();
        glShaderSource(fragmentShader,1,&glShaderSourceC,nullptr);
        Renderer::Shader::CompileShader(fragmentShader);
    }

    return true;

}


void Renderer::Shader::CompileShader(GLuint &shader)
{
    GLint param;

    glCompileShader(shader);
    glGetShaderiv(shader,GL_COMPILE_STATUS,&param);

    if(param != GL_TRUE)
    {
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&param);
        
        std::unique_ptr<char[]> log(new char[param]);

        glGetShaderInfoLog(shader,param,&param,log.get());
        Logger::Log(log.get(),Logger::ERROR);
    }


}

void Renderer::Shader::CreateProgram()
{
    shaderProgram = glCreateProgram();


	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

}

