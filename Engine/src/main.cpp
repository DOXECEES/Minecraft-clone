// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <GL/GL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/euler_angles.hpp>
#include <gtc/noise.hpp>
#include <iostream>
#include <memory>
// #include <GL/glew.h>

#include "Render/Window.hpp"
#include "Render/VertexArray.hpp"
#include "Render/VertexBuffer.hpp"
#include "Render/Shader.hpp"
#include "Render/Mesh.hpp"
#include "Render/Batch.hpp"
#include "Render/ChunksRenderer.hpp"
#include "World/Chunk.hpp"
#include "World/World.hpp"
//
#include "Render/Block.hpp"
//
#include "Camera.hpp"

#include "Logger.hpp"
#include "Templates/Array3D.hpp"

#include "../dependencies/lodepng/lodepng.h"

void error_callback(int error, const char *description)
{
    std::cout << "[ERROR CODE]: " << error << std::endl
              << "[DESCRIPTION] " << description << std::endl;
}

float skyboxVertices[] =
    {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

unsigned int skyboxIndices[] =
    {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3};

glm::vec3 camPos(0.0f, 2.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 dir;

void GLAPIENTRY
MessageCallback(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar *message,
                const void *userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

glm::vec3 cameraPos(0.0f, 0.0f, 0.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

void DrawLine(glm::vec3 start, glm::vec3 end, Renderer::Shader *sh)
{
    GLuint VAO;
    GLuint VBO;

    std::vector<float> vertices = {
        start.x,
        start.y,
        start.z,
        end.x,
        end.y,
        end.z,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    sh->Use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
}

int main()
{
    setlocale(LC_ALL, "ru");

    Logger::EnableConsoleLogging();
    Logger::StartLogging();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    Logger::Log("Starting...", Logger::INFO);

    // glfwSetErrorCallback(error_callback);

    Renderer::Window *wnd = new Renderer::Window(1920, 1080, "ass");

    glViewport(0, 0, 1920, 1080);

    Renderer::Shader s = Renderer::Shader();
    s.LoadShader("res\\Shaders\\ver.vs");
    s.LoadShader("res\\Shaders\\frag.fs");
    s.CreateProgram();

    glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(MessageCallback, 0);

    Renderer::Texture tex;
    tex.Bind();
    tex.LoadTexture("res\\textures\\block.png");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    auto camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));

    const size_t X = 16;
    const size_t Y = 256;
    const size_t Z = 16;

    auto world = new World();

    //

    bool isInfoMenuEnabled = false;
    auto batch = Renderer::Batch(&tex);
    auto renderer = Renderer::ChunksRenderer();

    glm::vec3 curDirection = cameraPos;
    glm::vec3 prevDirection = curDirection;
    Chunk *frontChunk = nullptr;

    auto a = world->GetChunkByLocal(Coordinates(1, 0, 1));

    auto saveCam = cameraPos;
    auto saveDir = dir;
    glm::vec3 dirVec = {};
    auto saveDirVec = dirVec;
    auto clik = false;

    while (wnd->Render())
    {
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera
        prevDirection = curDirection;

        glm::mat4 view = camera->GetViewMatrix();
        glm::vec3 cameraPos = camera->GetPosition();
        glm::vec3 cameraFront = camera->GetFront();

        auto dir = camera->GetDirection();

        auto blockCoords = Coordinates();

        /// TODO RAY CAST OBJECTS

        for (int i = 0; i < 1; i++)
        {
            auto t = std::to_string(cameraPos.x / 16.0f) + " " + std::to_string(cameraPos.y / 256.0f) + " " + std::to_string(cameraPos.z / 16.0f);

            if (world->GetBlockByGlobal(Coordinates(cameraPos.x + (dirVec.x * 5), cameraPos.y + (dirVec.y * 5), cameraPos.z + (dirVec.z * 5))) != Renderer::Block::BlockType::AIR)
            {
                blockCoords = Coordinates(cameraPos.x + (dirVec.x * 5), cameraPos.y + (dirVec.y * 5), cameraPos.z + (dirVec.z * 5));
                t += " BLOCK";
                glfwSetWindowTitle(wnd->GetWindow(), t.c_str());
            }
            else
            {
                t += " NOBLOC";
                glfwSetWindowTitle(wnd->GetWindow(), t.c_str());
            }

            // if (auto chunk = world->GetChunkByGlobal(Coordinates(cameraPos.x + dir.x + i, cameraPos.y + dir.y + i, cameraPos.z + dir.z + i)))
            // {
            //     if (chunk.has_value())
            //     {
            //         Logger::Log(t, Logger::INFO);
            //         if (chunk.value()->GetChunk()(abs((cameraPos.x / 16.0f + dir.x + i)), abs(cameraPos.y / 256.0f + dir.y), abs(cameraPos.z / 16.0f + dir.z + i)).GetTypeUInt() != 0)
            //         {
            //             chunk.value()->DeleteBlock(Coordinates(abs((cameraPos.x / 16.0f + dir.x + i)), abs(cameraPos.y / 256.0f + dir.y), abs(cameraPos.z / 16.0f + dir.z + i)));
            //             t += " BLOCK";
            //             glfwSetWindowTitle(wnd->GetWindow(), t.c_str());
            //             break;
            //         }
            //         else
            //         {
            //             t += " NOBLOC";
            //             glfwSetWindowTitle(wnd->GetWindow(), t.c_str());
            //         }
            //     }
            // }
        }

        curDirection = cameraPos;
        {
            frontChunk = *world->GetChunkByGlobal({cameraPos.x, cameraPos.y, cameraPos.z});
            // Logger::Log(std::to_string(static_cast<int>(cameraPos.x)), Logger::INFO);
            // Logger::Log(std::to_string(static_cast<int>(cameraPos.y)), Logger::INFO);
            // Logger::Log(std::to_string(static_cast<int>(cameraPos.z)), Logger::INFO);
        }

        // get movement direction

        // enum class Direction
        // {
        //     NONE = 0x0000,
        //     NORTH = 0x0001,
        //     NORTH_WEST = 0x0101,
        //     NORTH_EAST = 0x1001,
        //     SOUTH = 0x0010,
        //     SOUTH_WEST = 0x0110,
        //     SOUTH_EAST = 0x1010,
        //     WEST = 0x0100,
        //     EAST = 0x1000
        // };

        // std::bitset<4> d;
        // auto movementVector = curDirection - prevDirection;
        // if (movementVector != glm::vec3{0.0f, 0.0f, 0.0f})
        // {
        //     (movementVector.x > 0) ? Direction::EAST : Direction::WEST;
        //     (movementVector.y > 0) ? true : false;
        //     (movementVector.z > 0) ? Direction::NORTH : Direction::SOUTH;
        // }

        // 16 x 256 x 16

        auto chunkX = cameraPos.x / 16;
        auto chunkY = cameraPos.y / 256;
        auto chunkZ = cameraPos.z / 16;

        bool drawDir = false;

        if (wnd->IsKeyPressed(GLFW_KEY_A))
        {
            // camPos = camPos + glm::vec3(-0.01f, 0.0f, 0.0f);
            cameraPos = cameraPos - glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * 0.1f;
        }
        if (wnd->IsKeyPressed(GLFW_KEY_D))
        {
            cameraPos = cameraPos + glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * 0.1f;
        }
        if (wnd->IsKeyPressed(GLFW_KEY_S))
        {
            // camPos = camPos + glm::vec3(0.0f, 0.0f, 0.01f);
            cameraPos = cameraPos - cameraFront * 0.1f;
        }
        if (wnd->IsKeyPressed(GLFW_KEY_W))
        {
            // camPos = camPos + glm::vec3(0.0f, 0.0f, -0.01f);
            cameraPos = cameraPos + cameraFront * 0.1f;
        }
        if (wnd->IsKeyPressed(GLFW_KEY_SPACE))
        {
            cameraPos += glm::vec3(0.0f, 0.1f, 0.0f);
        }
        if (wnd->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
        {
            cameraPos -= glm::vec3(0.0f, 0.1f, 0.0f);
        }
        if (wnd->IsKeyPressed(GLFW_KEY_F5))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (wnd->IsKeyPressed(GLFW_KEY_F6))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
        if (wnd->IsKeyPressed(GLFW_KEY_F7))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if (wnd->IsKeyPressed(GLFW_KEY_F3))
        {
            isInfoMenuEnabled = !isInfoMenuEnabled;
        }
        if (wnd->IsKeyPressed(GLFW_KEY_F4))
        {
            drawDir = !drawDir;
            if (!clik)
            {
                world->DeleteBlockByGlobal(Coordinates(16.0f, 4.0f, -16.0f));
                clik = true;
            }
        }
        if (wnd->IsKeyPressed(GLFW_KEY_Q))
        {
            world->DeleteBlockByGlobal(blockCoords);
        }

        if (drawDir)
        {
            saveCam = cameraPos;
            saveDir = dir;
            saveDirVec = dirVec;
        }
        DrawLine(saveCam, saveCam + glm::vec3{saveDirVec.x * 5, saveDirVec.y * 5, saveDirVec.z * 5}, &s);

        camera->Update(cameraPos, wnd->input->mouse->GetX(), wnd->input->mouse->GetY());
        // std::string title;
        //  if (frontChunk != nullptr)
        //      title = std::to_string(frontChunk->GetLocalCoordinates().x) + " " + std::to_string(frontChunk->GetLocalCoordinates().y) + " " + std::to_string(frontChunk->GetLocalCoordinates().z);
        //  // auto title = "";
        //  title += "\t" + std::to_string(cameraPos.x) + " " + std::to_string(cameraPos.y) + " " + std::to_string(cameraPos.z);

        auto camX = ceil(cameraPos.x / 16);
        auto camZ = ceil(cameraPos.z / 16);

        dirVec = dir - cameraPos;

        // auto chunk = &chunks[camX + chunksSize * camZ];

        // glm::vec3 left;
        // if (chunk->left != nullptr)
        //     left = chunk->left->GetPosition();
        // else
        // {
        //     Logger::Log("no left", Logger::INFO);
        // }
        // auto right = chunk->right->GetPosition();
        // auto up = chunk->up->GetPosition();
        // auto down = chunk->down->GetPosition();

        // auto title = "LEFT: " + std::to_string(left.x) + " " + std::to_string(left.y) + " " + std::to_string(left.z) + " CUR: " + std::to_string(camX) + " " + std::to_string(0) + " " + std::to_string(camZ);
        // glfwSetWindowTitle(wnd->GetWindow(), title.c_str());
        //

        // Projection
        glm::mat4 projection;
        projection = glm::perspective(90.0f, (GLfloat)1920 / (GLfloat)1080, 0.1f, 100.0f);

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(s.GetProgram(), "model");
        GLint viewLoc = glGetUniformLocation(s.GetProgram(), "view");
        GLint projLoc = glGetUniformLocation(s.GetProgram(), "projection");
        GLint texLoc = glGetUniformLocation(s.GetProgram(), "ourTexture");

        // Pass the matrices to the shader
        glUniform1i(texLoc, 0);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model;
        model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        tex.Bind();
        auto chunk = world->GetRawChunks();
        // for (auto i = 0; i < chunk.Size(); i++)
        //     renderer.render(&batch, &chunk[i], &s);

        if (isInfoMenuEnabled)
        {
            // coordinates are not integers because the coordinate points to the center of the block
            for (auto &i : chunk)
            {
                auto coords = Chunk::ToGlobal(i.first);
                DrawLine(glm::vec3{coords.x - 0.5f, 0.0f, coords.z - 0.5f}, glm::vec3{coords.x - 0.5f, 256.0f, coords.z - 0.5f}, &s);
                DrawLine(glm::vec3{coords.x + 15.5f, 0.0f, coords.z - 0.5f}, glm::vec3{coords.x + 15.5f, 256.0f, coords.z - 0.5f}, &s);
                DrawLine(glm::vec3{coords.x - 0.5f, 0.0f, coords.z + 15.5f}, glm::vec3{coords.x - 0.5f, 256.0f, coords.z + 15.5f}, &s);
                DrawLine(glm::vec3{coords.x + 15.5f, 0.0f, coords.z + 15.5f}, glm::vec3{coords.x + 15.5f, 256.0f, coords.z + 15.5f}, &s);
            }
        }
        int i = 0;
        int j = 4;
        for (auto &[coords, chunk] : chunk)
        {
            auto batch = Renderer::Batch(&tex);
            renderer.render(&batch, chunk, &s, i);
            i++;
        }
    }
    glfwTerminate();
}
