
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
        std::vector<GLfloat> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<GLfloat> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> &textures);
        ~Mesh();
        void Draw(Shader &shader);

    private:
        Renderer::VertexArray *vao;

        void setupMesh();
    };
};