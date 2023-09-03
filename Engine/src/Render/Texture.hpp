#pragma once 

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Renderer
{

    class Texture
    {
        public:
            Texture();
            ~Texture();    

            void Bind();


        private:
            GLuint texture;

    };


};
