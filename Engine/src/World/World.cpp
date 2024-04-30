
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

    SetNeighbours();
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
            auto global = Chunk::ToGlobal(coords);
            int worldPosX = x + global.x;
            int worldPosZ = z + global.z;

            auto h = 20.0f + glm::perlin(glm::vec2(worldPosX / 16.f, worldPosZ / 16.f)) * 10;

            for (int y = 0; y < Chunk::Y; y++)
            {
                // chunk(x, y, z).SetType(Renderer::Block::BlockType::COBBLESTONE);

                if (y < h)
                {
                    if (y < h - 3)
                    {
                        chunk->SetBlock(Coordinates(x, y, z), Renderer::Block::BlockType::COBBLESTONE);
                    }
                    else
                    {
                        chunk->SetBlock(Coordinates(x, y, z), Renderer::Block::BlockType::GRASS);
                    }
                }
            }
        }
    }
    chunks[coords] = chunk;
}

void World::SetNeighbours()
{
    // for (int x = 0; x < 3; x++)
    // {
    //     for (int y = 0; y < 3; y++)
    //     {
    //         for (int z = 0; z < 3; z++)
    //         {

    //             if (IsInsideActiveChunks(Coordinates(x, y,z+1)))
    //             {
    //                 if (chunks[x + chunksSize * y].right == nullptr)
    //                 {
    //                     chunks[x + chunksSize * y].right = &chunks[x + chunksSize * (y + 1)];
    //                     chunks[x + chunksSize * (y + 1)].left = &chunks[x + chunksSize * y];
    //                 }
    //             }

    //             if (IsInsideActiveChunks(Coordinates(x, y, z- 1)))
    //             {
    //                 if (chunks[x + chunksSize * y].left == nullptr)
    //                 {
    //                     chunks[x + chunksSize * y].left = &chunks[x + chunksSize * (y - 1)];
    //                     chunks[x + chunksSize * (y - 1)].right = &chunks[x + chunksSize * y];
    //                 }
    //             }

    //             if (IsInsideActiveChunks(Coordinates(x + 1, y,z)))
    //             {
    //                 if (chunks[x + chunksSize * y].up == nullptr)
    //                 {
    //                     chunks[x + chunksSize * y].up = &chunks[(x + 1) + chunksSize * y];
    //                     chunks[(x + 1) + chunksSize * y].down = &chunks[x + chunksSize * y];
    //                 }
    //             }

    //             if (IsInsideActiveChunks(Coordinates(x - 1, y,z)))
    //             {
    //                 if (chunks[Coordinates(x + activeChunksSize * y)]->down == nullptr)
    //                 {
    //                     chunks[{x + activeChunksSize * y}]->down = &chunks[(x - 1) + chunksSize * y];
    //                     chunks[(x - 1) + chunksSize * y].up = &chunks[x + chunksSize * y];
    //                 }
    //             }
    //         }
    //     }
    // }

    for (auto &i : chunks)
    {
        auto coords = i.first;
        if (chunks.find(Coordinates(coords.x + 1.0f, coords.y, coords.z)) != chunks.end())
        {
            i.second->right = *GetChunkByLocal(Coordinates(coords.x + 1.0f, coords.y, coords.z));
        }
        if (chunks.find(Coordinates(coords.x - 1.0f, coords.y, coords.z)) != chunks.end())
        {
            i.second->left = *GetChunkByLocal(Coordinates(coords.x - 1.0f, coords.y, coords.z));
        }
        if (chunks.find(Coordinates(coords.x, coords.y, coords.z - 1.0f)) != chunks.end())
        {
            i.second->up = *GetChunkByLocal(Coordinates(coords.x, coords.y, coords.z - 1.0f));
        }
        if (chunks.find(Coordinates(coords.x, coords.y, coords.z + 1.0f)) != chunks.end())
        {
            i.second->down = *GetChunkByLocal(Coordinates(coords.x, coords.y, coords.z + 1.0f));
        }
    }
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
    if (chunks.find(Coordinates{floor(coords.x / Chunk::X), floor(coords.y / Chunk::Y), floor(coords.z / Chunk::Z)}) != chunks.end())
    {
        return chunks[Coordinates{floor(coords.x / Chunk::X), floor(coords.y / Chunk::Y), floor(coords.z / Chunk::Z)}];
    }
    // return chunks[Coordinates{floor(coords.x / Chunk::X), floor(coords.y / Chunk::Y), floor(coords.z / Chunk::Z)}];

    return {};
}

Renderer::Block::BlockType World::GetBlockByGlobal(Coordinates coords)
{
    auto local = Chunk::ToLocal(coords);

    auto chunk = GetChunkByGlobal(coords);

    if (chunk.has_value())
    {
        auto a = (chunk.value()->GetChunk()({local.second.x, local.second.y, local.second.z})).GetType();

        return a;
    }
    return Renderer::Block::BlockType::AIR;
}

bool World::IsChunkInsideActiveChunks(const Coordinates &coords) const
{
    return ((coords.x >= leftUpActiveChunkPosition.x && coords.x < rightBottomActiveChunkPosition.x) && (coords.y >= leftUpActiveChunkPosition.y && coords.y < rightBottomActiveChunkPosition.y) && (coords.z >= leftUpActiveChunkPosition.z && coords.z < rightBottomActiveChunkPosition.z));
}

void World::DeleteBlockByGlobal(Coordinates coords)
{
    auto local = Chunk::ToLocal(coords);
    auto chunk = GetChunkByLocal(local.first);
    if (chunk.has_value())
    {
        chunk.value()->DeleteBlock(local.second);
    }
}

World *World::worldInstance = nullptr;