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

bool isChunkInsideActiveChunks(int x, int y)
{
    return ((x >= 0 && x < 3) && (y >= 0 && y < 3));
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

    glfwSetErrorCallback(error_callback);

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
    // glEnable(GL_CULL_FACE);

    auto camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));

    // Array3D<Renderer::Block> chunk(96, 256, 96);
    const size_t X = 16;
    const size_t Y = 256;
    const size_t Z = 16;

    std::vector<Chunk> chunks;
    int renderDistance = 1;
    int chunksSize = renderDistance * 2 + 1;
    for (int i = 0; i < chunksSize; i++)
    {
        for (int j = 0; j < chunksSize; j++)
        {
            chunks.emplace_back(Chunk({-renderDistance + i, 0.0f, renderDistance - j}));
        }
    }

    // set neighbour chunks
    /// seems its inorrect
    for (int x = 0; x < chunksSize; x++)
    {
        for (int y = 0; y < chunksSize; y++)
        {
            if (isChunkInsideActiveChunks(x, y + 1))
            {
                if (chunks[x + chunksSize * y].right == nullptr)
                {
                    chunks[x + chunksSize * y].right = &chunks[x + chunksSize * (y + 1)];
                    chunks[x + chunksSize * (y + 1)].left = &chunks[x + chunksSize * y];
                }
            }

            if (isChunkInsideActiveChunks(x, y - 1))
            {
                if (chunks[x + chunksSize * y].left == nullptr)
                {
                    chunks[x + chunksSize * y].left = &chunks[x + chunksSize * (y - 1)];
                    chunks[x + chunksSize * (y - 1)].right = &chunks[x + chunksSize * y];
                }
            }

            if (isChunkInsideActiveChunks(x + 1, y))
            {
                if (chunks[x + chunksSize * y].down == nullptr)
                {
                    chunks[x + chunksSize * y].down = &chunks[(x + 1) + chunksSize * y];
                    chunks[(x + 1) + chunksSize * y].up = &chunks[x + chunksSize * y];
                }
            }

            if (isChunkInsideActiveChunks(x - 1, y))
            {
                if (chunks[x + chunksSize * y].up == nullptr)
                {
                    chunks[x + chunksSize * y].up = &chunks[(x - 1) + chunksSize * y];
                    chunks[(x - 1) + chunksSize * y].down = &chunks[x + chunksSize * y];
                }
            }
        }
    }

    bool isInfoMenuEnabled = false;
    auto batch = Renderer::Batch(&tex);
    auto renderer = Renderer::ChunksRenderer();

    while (wnd->Render())
    {
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera
        glm::mat4 view = camera->GetViewMatrix();
        glm::vec3 cameraPos = camera->GetPosition();
        glm::vec3 cameraFront = camera->GetFront();

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

        camera->Update(cameraPos, wnd->input->mouse->GetX(), wnd->input->mouse->GetY());
        // auto title = std::to_string(ceil(cameraPos.x)) + " " + std::to_string(ceil(cameraPos.y)) + " " + std::to_string(ceil(cameraPos.z));

        auto camX = ceil(cameraPos.x / 16);
        auto camZ = ceil(cameraPos.z / 16);

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
        for (auto &i : chunks)
            renderer.render(&batch, &i, &s);

        if (isInfoMenuEnabled)
        {
            std::string str = "dsa";
            // Set up the 2D rendering matrix
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, 1920, 0, 1080, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Set up the font and color
            glColor3f(1, 1, 1);
            glRasterPos2f(100, 100);
            for (int i = 0; i < str.size(); i++)
            {
                glBegin(GL_LINE);
                glVertex2d(0.0f, 0.0f);
                glVertex2d(0.0f, 1.0f);
                glEnd();
            }
        }
    }
    glfwTerminate();
}
