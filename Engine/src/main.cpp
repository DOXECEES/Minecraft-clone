#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <GL/GL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/euler_angles.hpp>
#include <iostream>
#include <memory>
// #include <GL/glew.h>

#include "Render/Window.hpp"
#include "Render/VertexArray.hpp"
#include "Render/VertexBuffer.hpp"
#include "Render/Shader.hpp"
#include "Render/Mesh.hpp"
#include "Render/Batch.hpp"
#include "Camera.hpp"

#include "Logger.hpp"
#include "Templates/Array3D.hpp"

#include "../dependencies/lodepng/lodepng.h"

void error_callback(int error, const char *description)
{
    std::cout << "[ERROR CODE]: " << error << std::endl
              << "[DESCRIPTION] " << description << std::endl;
}

std::vector<GLfloat> cube_vertices = {
    // front
    -1, -1, -1, 0, 0,
    1, -1, -1, 1, 0,
    1, 1, -1, 2, 0,
    -1, 1, -1, 3, 0,
    -1, -1, -1, 4, 0,

    -1, -1, 1, 0, -1,
    1, -1, 1, 1, -1,
    1, 1, 1, 2, -1,
    -1, 1, 1, 3, -1,
    -1, -1, 1, 4, -1,

    -1, 1, -1, 0, 1,
    1, 1, -1, 1, 1,

    -1, 1, 1, 0, -2,
    1, 1, 1, 1, -2};

std::vector<GLfloat> vertices = {

    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // Top Right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Bottom Right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom Left
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // Bottom Left Vertex 2

    // right triangle
    //  8

    // left triangle

};
std::vector<GLuint> indices = {
    // First triangle
    0, 1, 3, // first triangle
    3, 2, 1
    // Triangle 1
    // Triangle 3

};

std::vector<GLuint> cube_elements = {
    // front
    0, 1, 5, 5, 1, 6,
    1, 2, 6, 6, 2, 7,
    2, 3, 7, 7, 3, 8,
    3, 4, 8, 8, 4, 9,
    10, 11, 0, 0, 11, 1,
    5, 6, 12, 12, 6, 13};

std::vector<glm::vec3> pos = {
    {-1.0f, 2.0f, -5.0f},
    {1.0f, 2.0, -5.0f},
    {4.0f, 2.0, -5.0f},
    {7.0f, 2.0, -5.0f}

};

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

bool isIn(size_t x, size_t y, size_t z)
{
    return ((x >= 0 && x <= 15) && (y >= 0 && y <= 15) && (z >= 0 && z <= 15)) ? true : false;
}

int main()
{

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
    glDebugMessageCallback(MessageCallback, 0);

    Renderer::Texture tex;
    tex.Bind();
    tex.LoadTexture("res\\textures\\cobblestone.png");
    std::vector<Renderer::Texture> text;
    text.push_back(tex);

    // Renderer::Mesh cube(cube_vertices, cube_elements, text);
    // Renderer::Mesh triangle(vertices, indices, text);

    Renderer::Texture bedrock;
    bedrock.Bind();
    bedrock.LoadTexture("res\\textures\\bedrock.png");
    std::vector<Renderer::Texture> textbedr;
    textbedr.push_back(bedrock);

    // Renderer::Mesh cubeBedrock(cube_vertices, cube_elements, textbedr);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);

    auto camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Logger::Log("SKB\n", Logger::ERROR);

    pos.clear();

    Array3D<int> chunk(16, 16, 16);
    const size_t X = 16;
    const size_t Y = 16;
    const size_t Z = 16;

    for (int y = 0; y < Y; y++)
    {
        for (int x = 0; x < X; x++)
        {
            for (int z = 0; z < Z; z++)
            {
                chunk(x, y, z) = 1;
            }
        }
    }
    Logger::Log("SKB\n", Logger::ERROR);

    chunk(12, 4, 6) = 0;

    while (wnd->Render())
    {

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        s.Use();

        // camera
        glm::mat4 view = camera->GetViewMatrix();
        glm::vec3 cameraPos = camera->GetPosition();
        glm::vec3 cameraFront = camera->GetFront();

        if (wnd->input->kbr->isPressed(GLFW_KEY_A))
        {
            // camPos = camPos + glm::vec3(-0.01f, 0.0f, 0.0f);
            cameraPos = cameraPos - glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * 0.1f;
        }
        if (wnd->input->kbr->isPressed(GLFW_KEY_D))
        {
            cameraPos = cameraPos + glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))) * 0.1f;
        }
        if (wnd->input->kbr->isPressed(GLFW_KEY_S))
        {
            // camPos = camPos + glm::vec3(0.0f, 0.0f, 0.01f);
            cameraPos = cameraPos - cameraFront * 0.1f;
        }
        if (wnd->input->kbr->isPressed(GLFW_KEY_W))
        {
            // camPos = camPos + glm::vec3(0.0f, 0.0f, -0.01f);
            cameraPos = cameraPos + cameraFront * 0.1f;
        }
        if (wnd->input->kbr->isPressed(GLFW_KEY_SPACE))
        {
            cameraPos += glm::vec3(0.0f, 0.1f, 0.0f);
        }
        if (wnd->input->kbr->isPressed(GLFW_KEY_F5))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (wnd->input->kbr->isPressed(GLFW_KEY_F6))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
        if (wnd->input->kbr->isPressed(GLFW_KEY_F7))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        camera->Update(cameraPos, wnd->input->mouse->GetX(), wnd->input->mouse->GetY());

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

        // palyer
        // Camera control
        auto batch = Renderer::Batch();

        for (int y = 0; y < Y; y++)
        {
            for (int x = 0; x < X; x++)
            {
                for (int z = 0; z < Z; z++)
                {
                    if (!(isIn(x, y + 1, z) && chunk(x, y + 1, z)))
                    {
                        batch.Face({x, y, z}, 1, 1, Renderer::Batch::Faces::TOP);
                    }
                    if (!(isIn(x, y - 1, z) && chunk(x, y - 1, z)))
                    {
                        batch.Face({x, y, z}, 1, 1, Renderer::Batch::Faces::BOT);
                    }
                    if (!(isIn(x - 1, y, z) && chunk(x - 1, y, z)))
                    {
                        batch.Face({x, y, z}, 1, 1, Renderer::Batch::Faces::LEFT);
                    }
                    if (!(isIn(x + 1, y, z) && chunk(x + 1, y, z)))
                    {
                        batch.Face({x, y, z}, 1, 1, Renderer::Batch::Faces::RIGHT);
                    }
                    if (!(isIn(x, y, z - 1) && chunk(x, y, z - 1)))
                    {
                        batch.Face({x, y, z}, 1, 1, Renderer::Batch::Faces::FRONT);
                    }
                    if (!(isIn(x, y, z + 1) && chunk(x, y, z + 1)))
                    {
                        batch.Face({x, y, z}, 1, 1, Renderer::Batch::Faces::BACK);
                    }
                }
            }
        }

        glm::mat4 model;
        model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // std::unique_ptr<Renderer::Mesh> mesh = std::make_unique<Renderer::Mesh>(vert, indices, textbedr);

        auto mesh = batch.GetMesh();
        tex.Bind();
        mesh->Draw(s);
        delete mesh;
        //
    }
    glfwTerminate();
}
