#pragma once

#include <optional>
#include <unordered_map>

#include "../Templates/Array3D.hpp"
#include "Coordinates.hpp"
#include "Chunk.hpp"

class World
{
public:
    World();
    ~World();

    std::optional<Chunk *> GetChunkByLocal(const Coordinates &coords);
    std::optional<Chunk *> GetChunkByGlobal(const Coordinates &coords);

    Coordinates GetChunkCoordinates() const;

    void ReloadActiveChunks();
    void GenerateChunk(const Coordinates &coords);

    inline void InsertChunkInsideActive(const Coordinates &coords, Chunk *ch) { chunks[coords] = ch; };
    void RemoveChunkFromActive(const Coordinates &coords);

    inline World *GetWorld() const { return worldInstance; };
    const std::unordered_map<Coordinates, Chunk *> &GetRawChunks() const { return chunks; };

private:
    enum class ShiftDirection : uint8_t
    {
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    bool IsChunkInsideActiveChunks(const Coordinates &coords) const;
    // void ShiftChunks(ShiftDirection dir);
    void Init();

private:
    std::unordered_map<Coordinates, Chunk *> chunks;

    Coordinates leftUpActiveChunkPosition;
    Coordinates rightBottomActiveChunkPosition;

    static World *worldInstance;
};