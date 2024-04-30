// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Chunk.hpp"

Chunk::Chunk(const glm::vec3 coords)
    : chunk(X, Y, Z), position(coords)
{
    // for (int x = 0; x < X; x++)
    // {
    //     for (int z = 0; z < Z; z++)
    //     {
    //         int worldX = x + position.x * X;
    //         int worldZ = z + position.z * Z;

    //         auto h = 20 + glm::perlin(glm::vec2((float)worldX / 16.f, (float)worldZ / 16.f)) * 10;

    //         for (int y = 0; y < Chunk::Y; y++)
    //         {
    //             int worldY = y + position.y * Y;

    //             // chunk(x, y, z).SetType(Renderer::Block::BlockType::COBBLESTONE);

    //             if (y < h)
    //             {
    //                 if (y < h - 3)
    //                 {
    //                     chunk(x, y, z).SetType(Renderer::Block::BlockType::COBBLESTONE);
    //                 }
    //                 else
    //                 {
    //                     chunk(x, y, z).SetType(Renderer::Block::BlockType::GRASS);
    //                 }
    //             }
    //             chunk(x, y, z).SetType(Renderer::Block::BlockType::GRASS);
    //         }
    //     }
    // }
}

Coordinates Chunk::ToGlobal(const Coordinates &coords) noexcept
{
    return Coordinates(coords.x * Chunk::X, coords.y * Chunk::Y, coords.z * Chunk::Z);
}

std::pair<Coordinates, Coordinates> Chunk::ToLocal(const Coordinates &coords) noexcept
{
    auto chunkCoords = Coordinates(floorf(coords.x / Chunk::X), floorf(coords.y / Chunk::Y), floorf(coords.z / Chunk::Z));

    auto borders = chunkCoords * Coordinates(Chunk::X, Chunk::Y, Chunk::Z);
    auto result = coords - borders;

    return {chunkCoords, {floorf(result.x), floorf(result.y), floorf(result.z)}};
}

void Chunk::SetModified(bool exp) noexcept
{
    isModified = exp;
}

const Array3D<Renderer::Block> &Chunk::GetChunk() const
{
    return chunk;
}

bool Chunk::IsBlockInsideChunk(const Coordinates &coords) noexcept
{
    return ((coords.x >= 0 && coords.x < Chunk::X) && (coords.y >= 0 && coords.y < Chunk::Y) && (coords.z >= 0 && coords.z < Chunk::Z));
}

Coordinates Chunk::GetGlobalCoordinates() const noexcept
{
    return Coordinates(position.x * X, position.y * Y, position.z * Z);
}
