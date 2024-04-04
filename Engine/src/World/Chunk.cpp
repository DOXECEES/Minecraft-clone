// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Chunk.hpp"

Chunk::Chunk(const glm::vec3 coords)
    : chunk(X, Y, Z), position(coords)
{
    for (int x = 0; x < X; x++)
    {
        for (int z = 0; z < Z; z++)
        {
            int worldX = x + position.x * X;
            int worldZ = z + position.z * Z;

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

bool Chunk::IsBlockInsideChunk(const uint32_t x, const uint32_t y, const uint32_t z) noexcept
{
    return ((x >= 0 && x < Chunk::X) && (y >= 0 && y < Chunk::Y) && (z >= 0 && z < Chunk::Z));
}
