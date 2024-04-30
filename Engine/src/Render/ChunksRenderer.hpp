#pragma once

#include "../World/Chunk.hpp"
#include "Batch.hpp"

namespace Renderer
{
    class ChunksRenderer
    {
    public:
        ChunksRenderer() : mesh(9, nullptr){

                           };
        void render(Renderer::Batch *batch, Chunk *ch, Renderer::Shader *shader, int index);

    private:
        bool RenderEdge(Chunk *chunk, Chunk *neighbour, const Coordinates &coords, Renderer::Batch::Faces face);
        bool IsChunkEdge(int x, int y, int z);
        bool IsFaceVisible(const Chunk *ch, const Coordinates &coords);

    private:
        std::vector<Mesh *> mesh;
    };
};