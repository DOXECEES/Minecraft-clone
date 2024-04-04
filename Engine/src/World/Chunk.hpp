#pragma once

#include <gtc/noise.hpp>
#include "../Templates/Array3D.hpp"
#include "../Render/Block.hpp"

class Chunk
{
public:
    constexpr static auto X = 16;
    constexpr static auto Y = 256;
    constexpr static auto Z = 16;

    Chunk(const glm::vec3 coords);
    void SetModified(bool exp) noexcept;
    const Array3D<Renderer::Block> &GetChunk() const;
    inline bool Modified() const noexcept { return isModified; };
    inline glm::vec3 GetPosition() const noexcept { return position; };

    static bool IsBlockInsideChunk(Coordinates) noexcept;

private:
    Array3D<Renderer::Block> chunk;
    glm::vec3 position;
    bool isModified = true;

public:
    Chunk *right = nullptr;
    Chunk *left = nullptr;
    Chunk *up = nullptr;
    Chunk *down = nullptr;
};
