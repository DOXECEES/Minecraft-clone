// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Block.hpp"

Renderer::Block::Block(BlockType type)
    : type(type)
{
}

void Renderer::Block::SetType(BlockType type) noexcept
{
    this->type = type;
}

Renderer::Block::BlockType Renderer::Block::GetType() const noexcept
{
    return type;
}

uint8_t Renderer::Block::GetTypeUInt() const noexcept
{
    return static_cast<uint8_t>(type);
}