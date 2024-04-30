// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "ChunksRenderer.hpp"

void Renderer::ChunksRenderer::render(Renderer::Batch *batch, Chunk *ch, Renderer::Shader *shader, int index)
{
    auto chunk = ch->GetChunk();

    if (!ch->Modified())
    {
        mesh[index]->Draw(*shader);
        return;
    }

    auto pos = ch->GetGlobalCoordinates();

    auto worldXOffset = pos.x;
    auto worldYOffset = pos.y;
    auto worldZOffset = pos.z;

    for (int y = 0; y < Chunk::Y; y++)
    {
        for (int z = 0; z < Chunk::Z; z++)
        {
            for (int x = 0; x < Chunk::X; x++)
            {
                if (!chunk(x, y, z).GetTypeUInt())
                    continue;

                if (IsFaceVisible(ch, Coordinates(x, y + 1, z)))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::TOP);
                }
                if (IsFaceVisible(ch, Coordinates(x, y - 1, z)))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::BOT);
                }
                if (RenderEdge(ch, ch->left, Coordinates(x, y, z), Renderer::Batch::Faces::LEFT))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::LEFT);
                }
                if (RenderEdge(ch, ch->right, Coordinates(x, y, z), Renderer::Batch::Faces::RIGHT))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::RIGHT);
                }
                if (RenderEdge(ch, ch->up, Coordinates(x, y, z), Renderer::Batch::Faces::FRONT))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::FRONT);
                }
                if (RenderEdge(ch, ch->down, Coordinates(x, y, z), Renderer::Batch::Faces::BACK))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::BACK);
                }
            }
        }
    }

    delete mesh[index];
    mesh[index] = batch->GetMesh();
    mesh[index]->Draw(*shader);

    ch->SetModified(false);
}

bool Renderer::ChunksRenderer::RenderEdge(Chunk *chunk, Chunk *neighbour, const Coordinates &coords, Renderer::Batch::Faces face)
{
    auto x = coords.x;
    auto y = coords.y;
    auto z = coords.z;
    switch (face)
    {
    case Renderer::Batch::Faces::LEFT:
    {
        if (!Chunk::IsBlockInsideChunk(Coordinates(x - 1, y, z)) && neighbour != nullptr)
        {
            return !neighbour->GetChunk()(15.0f, coords.y, coords.z).GetTypeUInt();
        }
        return IsFaceVisible(chunk, Coordinates(x - 1, y, z));
    }
    case Renderer::Batch::Faces::RIGHT:
    {
        if (!Chunk::IsBlockInsideChunk(Coordinates(x + 1, y, z)) && neighbour != nullptr)
        {
            return !neighbour->GetChunk()(0.0f, coords.y, coords.z).GetTypeUInt();
        }
        return IsFaceVisible(chunk, Coordinates(x + 1, y, z));
    }
    case Renderer::Batch::Faces::FRONT:
    {
        if (!Chunk::IsBlockInsideChunk(Coordinates(x, y, z - 1)) && neighbour != nullptr)
        {
            return !neighbour->GetChunk()(coords.x, coords.y, 15.0f).GetTypeUInt();
        }
        return IsFaceVisible(chunk, Coordinates(x, y, z - 1));
    }
    case Renderer::Batch::Faces::BACK:
    {
        if (!Chunk::IsBlockInsideChunk(Coordinates(x, y, z + 1)) && neighbour != nullptr)
        {
            return !neighbour->GetChunk()(coords.x, coords.y, 0.0f).GetTypeUInt();
        }
        return IsFaceVisible(chunk, Coordinates(x, y, z + 1));
    }
    default:
        return true;
    }
}

bool Renderer::ChunksRenderer::IsFaceVisible(const Chunk *ch, const Coordinates &coords)
{
    return ((!(Chunk::IsBlockInsideChunk(coords) && ch->GetChunk()(coords.x, coords.y, coords.z).GetTypeUInt())));
}

bool Renderer::ChunksRenderer::IsChunkEdge(int x, int y, int z)
{
    return ((x == 0 || x == 15) || (z == 0 || z == 15));
}
