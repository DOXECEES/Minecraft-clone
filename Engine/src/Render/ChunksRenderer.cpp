// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "ChunksRenderer.hpp"

void Renderer::ChunksRenderer::render(Renderer::Batch *batch, Chunk *ch, Renderer::Shader *shader)
{
    auto chunk = ch->GetChunk();

    if (!ch->Modified())
    {
        mesh->Draw(*shader);
        return;
    }

    auto pos = ch->GetPosition();

    auto worldXOffset = pos.x * Chunk::X;
    auto worldYOffset = pos.y * Chunk::Y;
    auto worldZOffset = pos.z * Chunk::Z;

    for (int y = 0; y < Chunk::Y; y++)
    {
        for (int z = 0; z < Chunk::Z; z++)
        {
            for (int x = 0; x < Chunk::X; x++)
            {
                if (!chunk(x, y, z).GetTypeUInt())
                    continue;

                if (IsFaceVisible(ch, x, y + 1, z))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::TOP);
                }
                if (IsFaceVisible(ch, x, y - 1, z))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::BOT);
                }
                if (IsFaceVisible(ch, x - 1, y, z) && !IsBlockInNeighbourChunk(ch, x, y, z, Renderer::Batch::Faces::LEFT))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::LEFT);
                }
                if (IsFaceVisible(ch, x + 1, y, z) && !IsBlockInNeighbourChunk(ch, x, y, z, Renderer::Batch::Faces::RIGHT))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::RIGHT);
                }
                if (IsFaceVisible(ch, x, y, z - 1) && !IsBlockInNeighbourChunk(ch, x, y, z, Renderer::Batch::Faces::FRONT))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::FRONT);
                }
                if (IsFaceVisible(ch, x, y, z + 1) && IsBlockInNeighbourChunk(ch, x, y, z, Renderer::Batch::Faces::BACK))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::BACK);
                }
            }
        }
    }

    delete mesh;
    mesh = batch->GetMesh();
    mesh->Draw(*shader);

    ch->SetModified(false);
}

bool Renderer::ChunksRenderer::IsFaceVisible(const Chunk *ch, int x, int y, int z)
{
    return ((!(IsBlockInsideChunk(x, y, z) && ch->GetChunk()(x, y, z).GetTypeUInt())));
}

bool Renderer::ChunksRenderer::IsBlockInNeighbourChunk(const Chunk *ch, int x, int y, int z, Renderer::Batch::Faces face)
{
    switch (face)
    {
    case Renderer::Batch::Faces::LEFT:
        return (!(IsBlockInsideChunk(x, y, z) && ch->right != nullptr && ch->right->GetChunk()(0, y, z).GetTypeUInt()));
    case Renderer::Batch::Faces::RIGHT:
        return (!(IsBlockInsideChunk(x, y, z) && ch->left != nullptr && ch->left->GetChunk()(15, y, z).GetTypeUInt()));
    case Renderer::Batch::Faces::FRONT:
        return (!(IsBlockInsideChunk(x, y, z) && ch->up != nullptr && ch->up->GetChunk()(x, y, 0).GetTypeUInt()));
    case Renderer::Batch::Faces::BACK:
        return (!(IsBlockInsideChunk(x, y, z) && ch->down != nullptr && ch->down->GetChunk()(x, y, 15).GetTypeUInt()));
    default:
        return true;
    }
}

bool Renderer::ChunksRenderer::IsChunkEdge(int x, int y, int z)
{
    return ((x == 0 || x == 15) || (z == 0 || z == 15));
}

bool Renderer::ChunksRenderer::IsBlockExists(Chunk *chunk, Renderer::Batch::Faces face, int x, int y, int z)
{
    if (chunk == nullptr)
        return false;

    auto ch = chunk->GetChunk();

    switch (face)
    {
    case Renderer::Batch::Faces::RIGHT:
        return static_cast<bool>(ch(y, 15, z).GetTypeUInt());

    case Renderer::Batch::Faces::LEFT:
        return static_cast<bool>(ch(y, 0, z).GetTypeUInt());

    case Renderer::Batch::Faces::FRONT:
        return static_cast<bool>(ch(y, x, 0).GetTypeUInt());

    case Renderer::Batch::Faces::BACK:
        return static_cast<bool>(ch(y, x, 15).GetTypeUInt());

    default:
        Logger::Log("Top and bot faces unvailiable", Logger::ERROR);
        return false;
    }
}
