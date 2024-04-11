
#include "World.hpp"
World::World()
{
    if (worldInstance == nullptr)
    {
        Init();
    }
}

void World::Init()
{
    int renderDistance = 1;
    int chunksSize = renderDistance * 2 + 1;
    // Array3D<Chunk> chunks(chunksSize, 1 , chunksSize);

    // 0 3 6
    // 1 4 7
    // 2 5 8
    Logger::Log("Gen chunk1", Logger::INFO);

    for (int i = 0; i < chunksSize; i++)
    {
        for (int j = 0; j < chunksSize; j++)
        {
            chunks[Coordinates(-renderDistance + i, 0.0f, renderDistance - j)] = nullptr;
            GenerateChunk(Coordinates(-renderDistance + i, 0.0f, renderDistance - j));
        }
    }
}

void World::GenerateChunk(const Coordinates &coords)
{
    auto chunk1 = GetChunkByLocal(coords);
    if (chunk1 != nullptr)
        return;

    auto chunk = new Chunk({coords.x, coords.y, coords.z});
    Logger::Log("Gen chunk", Logger::INFO);
    for (int x = 0; x < Chunk::X; x++)
    {
        for (int z = 0; z < Chunk::Z; z++)
        {
            auto worldPos = Chunk::ToGlobal(coords);

            auto h = 20 + glm::perlin(glm::vec2((float)worldPos.x / 16.f, (float)worldPos.z / 16.f)) * 10;

            for (int y = 0; y < Chunk::Y; y++)
            {
                // chunk(x, y, z).SetType(Renderer::Block::BlockType::COBBLESTONE);

                if (y < h)
                {
                    if (y < h - 3)
                    {
                        auto ch = chunk->GetChunk();
                        ch(x, y, z).SetType(Renderer::Block::BlockType::COBBLESTONE);
                    }
                    else
                    {
                        auto ch = chunk->GetChunk();
                        ch(x, y, z).SetType(Renderer::Block::BlockType::GRASS);
                    }
                }
            }
        }
    }
    chunks[coords] = chunk;
}

void World::RemoveChunkFromActive(const Coordinates &coords)
{
    auto it = chunks.find(coords);
    if (it != chunks.end())
    {
        chunks.erase(it);
    }
}

std::optional<Chunk *> World::GetChunkByLocal(const Coordinates &coords)
{
    // if (IsChunkInsideActiveChunks(coords))
    //     return chunks[coords];
    // else
    //     return {};
    return chunks[coords];
}

std::optional<Chunk *> World::GetChunkByGlobal(const Coordinates &coords)
{
    return chunks[Coordinates{static_cast<int>(floorl(coords.x / Chunk::X)), static_cast<int>(floor(coords.y / Chunk::Y)), static_cast<int>(floor(coords.z / Chunk::Z))}];
}

bool World::IsChunkInsideActiveChunks(const Coordinates &coords) const
{
    return ((coords.x >= leftUpActiveChunkPosition.x && coords.x < rightBottomActiveChunkPosition.x) && (coords.y >= leftUpActiveChunkPosition.y && coords.y < rightBottomActiveChunkPosition.y) && (coords.z >= leftUpActiveChunkPosition.z && coords.z < rightBottomActiveChunkPosition.z));
}

World *World::worldInstance = nullptr;