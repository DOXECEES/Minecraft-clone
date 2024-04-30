#pragma once

#include <gtc/noise.hpp>
#include "../Templates/Array3D.hpp"
#include "../Render/Block.hpp"
#include "../Render/Batch.hpp"

#include "Coordinates.hpp"

#include <bitset>

class Chunk
{
public:
    constexpr static auto X = 16.0f;
    constexpr static auto Y = 256.0f;
    constexpr static auto Z = 16.0f;

    Chunk(const glm::vec3 coords);
    void SetModified(bool exp) noexcept;
    const Array3D<Renderer::Block> &GetChunk() const;
    inline bool Modified() const noexcept { return isModified; };
    inline Coordinates GetLocalCoordinates() const noexcept { return Coordinates(position.x, position.y, position.z); };
    Coordinates GetGlobalCoordinates() const noexcept;

    inline void SetBlock(Coordinates coords, Renderer::Block::BlockType type) { chunk(coords).SetType(type); };
    inline void DeleteBlock(Coordinates coords)
    {
        chunk(coords).SetType(Renderer::Block::BlockType::AIR);
        isModified = true;
    };

    inline bool IsFaceExists(Renderer::Batch::Faces face) const { return existingFaces[static_cast<int>(face)]; };
    inline void SetFace(Renderer::Batch::Faces face) { existingFaces.set(static_cast<int>(face)); };

    static Coordinates ToGlobal(const Coordinates &coords) noexcept;

    /// @brief
    /// @param coords
    /// @return first param represents coordinate of chunk in world coords;
    /// @return second represents coordinate of block in this coords

    static std::pair<Coordinates, Coordinates> ToLocal(const Coordinates &coords) noexcept;
    static bool IsBlockInsideChunk(const Coordinates &coords) noexcept;

private:
    Array3D<Renderer::Block> chunk;
    glm::vec3 position;
    bool isModified = true;

    std::bitset<6> existingFaces;

public:
    Chunk *right = nullptr;
    Chunk *left = nullptr;
    Chunk *up = nullptr;
    Chunk *down = nullptr;
};
