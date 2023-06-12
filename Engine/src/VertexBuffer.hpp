
#include <glfw/glfw3.h>
#include <glad/glad.h>

namespace Render
{
    class VertexBuffer 
    {
    public:

        VertexBuffer();
        ~VertexBuffer();

    private:
        GLuint VBO;
        GLsizei ID;
    private:
        //static GLsizei freeId; // Free id in current moment 
    };

}