
#pragma once

#include <glm.hpp>

#include <cstdint>

#include "Mesh.hpp"

namespace Renderer
{
    class Batch
    {
    public:
        enum class Faces : uint8_t
        {
            FRONT,
            BACK,
            LEFT,
            RIGHT,
            TOP,
            BOT
        };

        Batch();
        void Vertex(const float x, const float y, const float z, const float u, const float v);
        void Face(const glm::vec3 coords, float u, float v, Faces face);
        void MakeIndiciesForQuad_CW();
        void MakeIndiciesForQuad_CCW();

        Mesh *GetMesh();

    private:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indicies;
        std::vector<Texture> text;
        uint32_t currentIndex = 0;
    };
};