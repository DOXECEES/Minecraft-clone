
#pragma once

#include <glm.hpp>

#include <cstdint>

#include "Mesh.hpp"
#include "Block.hpp"

namespace Renderer
{
    class Batch
    {
        constexpr static auto textureWidth = 32.0f;

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

        Batch(Texture *texture);
        void Vertex(const float x, const float y, const float z, const float u, const float v);
        void Face(const glm::vec3 coords, Renderer::Block::BlockType type, Faces face);
        Mesh *GetMesh();

    private:
        void MakeIndiciesForQuad_CW();
        void MakeIndiciesForQuad_CCW();

    private:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indicies;
        std::vector<Texture> text;
        float uvSize;
        uint8_t countOfTiles;
        uint8_t gridSize;
        float step;
        uint32_t currentIndex = 0;
    };
};