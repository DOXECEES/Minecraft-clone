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
        bool IsBlockInsideChunk(int x, int y, int z);

    private:
        Mesh *mesh;
    };
};