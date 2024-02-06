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

    Chunk(glm::vec3 coords);
    void SetModified(bool exp) noexcept;
    const Array3D<Renderer::Block> &GetChunk() const;
    inline bool Modified() const { return isModified; };

private:
    Array3D<Renderer::Block> chunk;
    bool isModified = true;
};
