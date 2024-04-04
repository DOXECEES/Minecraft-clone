#pragma once

#include "../World/Chunk.hpp"
#include "Batch.hpp"

namespace Renderer
{
    class ChunksRenderer
    {
    public:
        ChunksRenderer() = default;
        void render(Renderer::Batch *batch, Chunk *ch, Renderer::Shader *shader);

    private:
        bool IsBlockInsideChunk(int x, int y, int z);
        bool IsBlockExists(Chunk *chunk, Renderer::Batch::Faces face, int x, int y, int z);
        bool IsChunkEdge(int x, int y, int z);
        bool IsFaceVisible(const Chunk *ch, int x, int y, int z);
        bool IsBlockInNeighbourChunk(const Chunk *ch, int x, int y, int z, Renderer::Batch::Faces face);

    private:
        Mesh *mesh = nullptr;
    };
};