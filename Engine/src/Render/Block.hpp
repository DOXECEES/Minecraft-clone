#pragma once

#include <cstdint>

namespace Renderer
{
    class Block
    {
    public:
        enum class BlockType : uint8_t
        {
            AIR = 0,
            COBBLESTONE = 1,
            BEDROCK = 2,
        };

        Block(BlockType type = BlockType::AIR);
        void SetType(BlockType type) noexcept;
        BlockType GetType() const noexcept;
        uint8_t GetTypeUInt() const noexcept;

    private:
        BlockType type;
    };

}