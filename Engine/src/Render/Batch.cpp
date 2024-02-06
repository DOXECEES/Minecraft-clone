#include "Batch.hpp"

Renderer::Batch::Batch(Texture *texture)
{
    countOfTiles = texture->GetSquare() / (textureWidth * textureWidth);
    gridSize = static_cast<uint8_t>(sqrt(countOfTiles)); // number always will be 16, 64...
    step = 1.0f / static_cast<float>(gridSize);
}

void Renderer::Batch::Vertex(const float x, const float y, const float z, const float u, const float v)
{
    vertices.insert(vertices.end(), {x, y, z, u, v});
}

void Renderer::Batch::MakeIndiciesForQuad_CW()
{
    currentIndex += 4;
    indicies.insert(indicies.end(), {currentIndex - 4, currentIndex - 3, currentIndex - 2, currentIndex - 2, currentIndex - 1, currentIndex - 4});
}

void Renderer::Batch::MakeIndiciesForQuad_CCW()
{
    currentIndex += 4;
    indicies.insert(indicies.end(), {currentIndex - 2, currentIndex - 3, currentIndex - 4, currentIndex - 2, currentIndex - 4, currentIndex - 1});
}

void Renderer::Batch::Face(const glm::vec3 coords, Renderer::Block::BlockType type, Faces face)
{

    auto u = step * ((static_cast<uint8_t>(type) - 1) % gridSize);
    auto v = step * ((static_cast<uint8_t>(type) - 1) / gridSize);

    switch (face)
    {
    case Faces::FRONT:
    {
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v + step);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z - 0.5f, u + step, v + step);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z - 0.5f, u + step, v);
        MakeIndiciesForQuad_CW();
        break;
    }
    case Faces::BACK:
    {
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z + 0.5f, u, v + step);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z + 0.5f, u + step, v + step);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z + 0.5f, u + step, v);
        MakeIndiciesForQuad_CCW();
        break;
    }
    case Faces::LEFT:
    {
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v + step);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z + 0.5f, u + step, v + step);
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z + 0.5f, u + step, v);
        MakeIndiciesForQuad_CCW();
        break;
    }
    case Faces::RIGHT:
    {
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v + step);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z + 0.5f, u + step, v + step);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z + 0.5f, u + step, v);
        MakeIndiciesForQuad_CW();
        break;
    }
    case Faces::TOP:
    {
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z + 0.5f, u, v + step);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z + 0.5f, u + step, v + step);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z - 0.5f, u + step, v);
        MakeIndiciesForQuad_CW();
        break;
    }
    case Faces::BOT:
    {
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z + 0.5f, u, v + step);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z + 0.5f, u + step, v + step);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z - 0.5f, u + step, v);
        MakeIndiciesForQuad_CCW();
        break;
    }
    default:
        return;
    }
}

Renderer::Mesh *Renderer::Batch::GetMesh()
{
    Logger::Log(std::to_string(vertices.size()), Logger::INFO);
    return new Renderer::Mesh(vertices, indicies);
}