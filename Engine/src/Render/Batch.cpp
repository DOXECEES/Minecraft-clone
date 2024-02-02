#include "Batch.hpp"

Renderer::Batch::Batch()
{
}

void Renderer::Batch::Vertex(const float x, const float y, const float z, const float u, const float v)
{
    vertices.insert(vertices.end(), {x, y, z, u, v});
}

void Renderer::Batch::MakeIndiciesForQuad()
{
    currentIndex += 4;
    indicies.insert(indicies.end(), {currentIndex - 4, currentIndex - 3, currentIndex - 2, currentIndex - 2, currentIndex - 1, currentIndex - 4});
}

void Renderer::Batch::Face(const glm::vec3 coords, float u, float v, Faces face)
{
    switch (face)
    {
    case Faces::FRONT:
    {
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        break;
    }
    case Faces::BACK:
    {
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z + 0.5f, u, v);
        break;
    }
    case Faces::LEFT:
    {
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z + 0.5f, u, v);
        break;
    }
    case Faces::RIGHT:
    {
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z + 0.5f, u, v);
        break;
    }
    case Faces::TOP:
    {
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y + 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y + 0.5f, coords.z - 0.5f, u, v);
        break;
    }
    case Faces::BOT:
    {
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        Vertex(coords.x - 0.5f, coords.y - 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z + 0.5f, u, v);
        Vertex(coords.x + 0.5f, coords.y - 0.5f, coords.z - 0.5f, u, v);
        break;
    }
    default:
        break;
    }

    MakeIndiciesForQuad();
}

Renderer::Mesh *Renderer::Batch::GetMesh()
{
    Logger::Log(std::to_string(vertices.size()), Logger::INFO);
    return new Renderer::Mesh(vertices, indicies, text);
}