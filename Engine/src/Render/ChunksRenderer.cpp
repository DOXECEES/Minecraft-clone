#include "ChunksRenderer.hpp"

void Renderer::ChunksRenderer::render(Renderer::Batch *batch, Chunk *ch, Renderer::Shader *shader)
{
    auto chunk = ch->GetChunk();

    if (!ch->Modified())
    {
        mesh->Draw(*shader);
        return;
    }

    auto worldXOffset = ch->GetPosition().x * Chunk::X;
    auto worldYOffset = ch->GetPosition().y * Chunk::Y;
    auto worldZOffset = ch->GetPosition().z * Chunk::Z;

    for (int y = 0; y < Chunk::Y; y++)
    {
        for (int z = 0; z < Chunk::Z; z++)
        {
            for (int x = 0; x < Chunk::X; x++)
            {
                if (!chunk(x, y, z).GetTypeUInt())
                    continue;

                if (!(IsBlockInsideChunk(x, y + 1, z) && chunk(x, y + 1, z).GetTypeUInt()))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::TOP);
                }
                if (!(IsBlockInsideChunk(x, y - 1, z) && chunk(x, y - 1, z).GetTypeUInt()))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::BOT);
                }
                if (!(IsBlockInsideChunk(x - 1, y, z) && chunk(x - 1, y, z).GetTypeUInt()))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::LEFT);
                }
                if (!(IsBlockInsideChunk(x + 1, y, z) && chunk(x + 1, y, z).GetTypeUInt()))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::RIGHT);
                }
                if (!(IsBlockInsideChunk(x, y, z - 1) && chunk(x, y, z - 1).GetTypeUInt()))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::FRONT);
                }
                if (!(IsBlockInsideChunk(x, y, z + 1) && chunk(x, y, z + 1).GetTypeUInt()))
                {
                    batch->Face({x + worldXOffset, y + worldYOffset, z + worldZOffset}, chunk(x, y, z).GetType(), Renderer::Batch::Faces::BACK);
                }
            }
        }
    }

    if (mesh != nullptr)
        delete mesh;

    mesh = batch->GetMesh();
    mesh->Draw(*shader);

    ch->SetModified(false);
}

bool Renderer::ChunksRenderer::IsBlockInsideChunk(int x, int y, int z)
{
    return ((x >= 0 && x < Chunk::X) && (y >= 0 && y < Chunk::Y) && (z >= 0 && z < Chunk::Z));
}