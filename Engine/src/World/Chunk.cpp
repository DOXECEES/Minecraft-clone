
#include "Chunk.hpp"

Chunk::Chunk(glm::vec3 coords)
    : chunk(X, Y, Z)
{
    for (int x = 0; x < X; x++)
    {
        for (int z = 0; z < Z; z++)
        {
            int worldX = x + coords.x * X;
            int worldZ = z + coords.z * Z;

            auto h = 20 + glm::perlin(glm::vec2((float)worldX / 16.f, (float)worldZ / 16.f)) * 10;

            for (int y = 0; y < Y; y++)
            {
                if (y < h)
                {
                    if (y < h - 3)
                    {
                        chunk(x, y, z).SetType(Renderer::Block::BlockType::COBBLESTONE);
                    }
                    else
                    {
                        chunk(x, y, z).SetType(Renderer::Block::BlockType::GRASS);
                    }
                }
            }
        }
    }
}

void Chunk::SetModified(bool exp) noexcept
{
    isModified = exp;
}

const Array3D<Renderer::Block> &Chunk::GetChunk() const
{
    return chunk;
}
