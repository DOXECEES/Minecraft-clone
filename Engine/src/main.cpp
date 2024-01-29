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

    // chunk(1, 0, 0) = 0;

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
        glUniform1ui(texLoc, 0);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // palyer
        // Camera control

        std::vector<GLuint> indices;
        std::vector<float> vert;
        int curIndex = 0;
        for (int y = 0; y < Y; y++)
        {
            for (int x = 0; x < X; x++)
            {
                for (int z = 0; z < Z; z++)
                {
                    bool isTopExists = false;
                    bool isBottomExists = false;

                    if (!(isIn(x, y + 1, z) && chunk(x, y + 1, z)))
                    {

                        // generate indicies
                        // 012230
                        indices.push_back(curIndex);
                        curIndex++;
                        indices.push_back(curIndex);
                        curIndex++;
                        indices.push_back(curIndex);
                        indices.push_back(curIndex);
                        curIndex++;
                        indices.push_back(curIndex - 3);
                        indices.push_back(curIndex);
                        curIndex++;

                        vert.push_back(x - 0.5f);
                        vert.push_back(y + 0.5f);
                        vert.push_back(z - 0.5f);

                        vert.push_back(1);
                        vert.push_back(1);

                        vert.push_back(x - 0.5f);
                        vert.push_back(y + 0.5f);
                        vert.push_back(z + 0.5f);

                        vert.push_back(1);
                        vert.push_back(0);

                        vert.push_back(x + 0.5f);
                        vert.push_back(y + 0.5f);
                        vert.push_back(z + 0.5f);

                        vert.push_back(0);
                        vert.push_back(0);

                        vert.push_back(x + 0.5f);
                        vert.push_back(y + 0.5f);
                        vert.push_back(z - 0.5f);

                        vert.push_back(0);
                        vert.push_back(1);

                        isTopExists = true;
                    }
                    if (!(isIn(x, y - 1, z) && chunk(x, y - 1, z)))
                    {
                        indices.push_back(curIndex);
                        curIndex++;
                        indices.push_back(curIndex);
                        curIndex++;
                        indices.push_back(curIndex);
                        indices.push_back(curIndex);
                        curIndex++;
                        indices.push_back(curIndex - 3);
                        indices.push_back(curIndex);
                        curIndex++;

                        vert.push_back(x - 0.5f);
                        vert.push_back(y - 0.5f);
                        vert.push_back(z - 0.5f);

                        vert.push_back(1);
                        vert.push_back(1);

                        vert.push_back(x - 0.5f);
                        vert.push_back(y - 0.5f);
                        vert.push_back(z + 0.5f);

                        vert.push_back(1);
                        vert.push_back(0);

                        vert.push_back(x + 0.5f);
                        vert.push_back(y - 0.5f);
                        vert.push_back(z + 0.5f);

                        vert.push_back(0);
                        vert.push_back(0);

                        vert.push_back(x + 0.5f);
                        vert.push_back(y - 0.5f);
                        vert.push_back(z - 0.5f);

                        vert.push_back(0);
                        vert.push_back(1);

                        isBottomExists = true;
                    }
                    if (!(isIn(x - 1, y, z) && chunk(x - 1, y, z)))
                    {
                        if (isTopExists && isBottomExists)
                        {
                            indices.push_back(curIndex - 7);
                            indices.push_back(curIndex - 6);
                            indices.push_back(curIndex - 2);
                            indices.push_back(curIndex - 2);
                            indices.push_back(curIndex - 3);
                            indices.push_back(curIndex - 7);
                        }
                        else if (isTopExists)
                        {
                            indices.push_back(curIndex - 4);
                            indices.push_back(curIndex - 3);
                            indices.push_back(curIndex);
                            indices.push_back(curIndex);
                            curIndex++;
                            indices.push_back(curIndex);
                            indices.push_back(curIndex - 5);
                            curIndex++;

                            vert.push_back(x - 0.5f);
                            vert.push_back(y - 0.5f);
                            vert.push_back(z + 0.5f);

                            vert.push_back(0);
                            vert.push_back(0);

                            vert.push_back(x - 0.5f);
                            vert.push_back(y - 0.5f);
                            vert.push_back(z - 0.5f);

                            vert.push_back(0);
                            vert.push_back(1);
                        }
                        else if (isBottomExists)
                        {
                            indices.push_back(curIndex - 4);
                            indices.push_back(curIndex - 3);
                            indices.push_back(curIndex);
                            indices.push_back(curIndex);
                            curIndex++;
                            indices.push_back(curIndex);
                            indices.push_back(curIndex - 5);
                            curIndex++;

                            vert.push_back(x - 0.5f);
                            vert.push_back(y + 0.5f);
                            vert.push_back(z + 0.5f);

                            vert.push_back(0);
                            vert.push_back(0);

                            vert.push_back(x - 0.5f);
                            vert.push_back(y + 0.5f);
                            vert.push_back(z - 0.5f);

                            vert.push_back(0);
                            vert.push_back(1);
                        }
                        else
                        {
                            indices.push_back(curIndex);
                            curIndex++;
                            indices.push_back(curIndex);
                            curIndex++;
                            indices.push_back(curIndex);
                            indices.push_back(curIndex);
                            curIndex++;
                            indices.push_back(curIndex - 3);
                            indices.push_back(curIndex);
                            curIndex++;

                            vert.push_back(x - 0.5f);
                            vert.push_back(y + 0.5f);
                            vert.push_back(z - 0.5f);

                            vert.push_back(1);
                            vert.push_back(1);

                            vert.push_back(x - 0.5f);
                            vert.push_back(y + 0.5f);
                            vert.push_back(z + 0.5f);

                            vert.push_back(1);
                            vert.push_back(0);

                            vert.push_back(x - 0.5f);
                            vert.push_back(y - 0.5f);
                            vert.push_back(z + 0.5f);

                            vert.push_back(0);
                            vert.push_back(0);

                            vert.push_back(x - 0.5f);
                            vert.push_back(y - 0.5f);
                            vert.push_back(z - 0.5f);

                            vert.push_back(0);
                            vert.push_back(1);
                        }
                    }
                    if (!(isIn(x + 1, y, z) && chunk(x + 1, y, z)))
                    {
                        if (isTopExists && isBottomExists)
                        {
                            // indices.push_back(curIndex - 6);
                            // indices.push_back(curIndex - 5);
                            // indices.push_back(curIndex - 1);
                            // indices.push_back(curIndex - 1);
                            // indices.push_back(curIndex - 2);
                            // indices.push_back(curIndex - 6);
                        }
                        else if (isTopExists)
                        {
                            indices.push_back(curIndex - 1);
                            indices.push_back(curIndex - 2);
                            curIndex++;
                            indices.push_back(curIndex);
                            indices.push_back(curIndex);
                            indices.push_back(curIndex - 1);
                            indices.push_back(curIndex - 3);
                            curIndex++;

                            vert.push_back(x + 0.5f);
                            vert.push_back(y - 0.5f);
                            vert.push_back(z + 0.5f);

                            vert.push_back(1);
                            vert.push_back(0);

                            vert.push_back(x + 0.5f);
                            vert.push_back(y - 0.5f);
                            vert.push_back(z - 0.5f);

                            vert.push_back(1);
                            vert.push_back(1);
                        }
                        // else if (isBottomExists)
                        // {
                        //     indices.push_back(curIndex - 4);
                        //     indices.push_back(curIndex - 3);
                        //     indices.push_back(curIndex);
                        //     indices.push_back(curIndex);
                        //     curIndex++;
                        //     indices.push_back(curIndex);
                        //     indices.push_back(curIndex - 5);
                        //     curIndex++;

                        //     vert.push_back(x - 0.5f);
                        //     vert.push_back(y + 0.5f);
                        //     vert.push_back(z + 0.5f);

                        //     vert.push_back(0);
                        //     vert.push_back(0);

                        //     vert.push_back(x - 0.5f);
                        //     vert.push_back(y + 0.5f);
                        //     vert.push_back(z - 0.5f);

                        //     vert.push_back(0);
                        //     vert.push_back(1);
                        // }
                        // else
                        // {
                        //     indices.push_back(curIndex);
                        //     curIndex++;
                        //     indices.push_back(curIndex);
                        //     curIndex++;
                        //     indices.push_back(curIndex);
                        //     indices.push_back(curIndex);
                        //     curIndex++;
                        //     indices.push_back(curIndex - 3);
                        //     indices.push_back(curIndex);
                        //     curIndex++;

                        //     vert.push_back(x - 0.5f);
                        //     vert.push_back(y + 0.5f);
                        //     vert.push_back(z - 0.5f);

                        //     vert.push_back(1);
                        //     vert.push_back(1);

                        //     vert.push_back(x - 0.5f);
                        //     vert.push_back(y + 0.5f);
                        //     vert.push_back(z + 0.5f);

                        //     vert.push_back(1);
                        //     vert.push_back(0);

                        //     vert.push_back(x - 0.5f);
                        //     vert.push_back(y - 0.5f);
                        //     vert.push_back(z + 0.5f);

                        //     vert.push_back(0);
                        //     vert.push_back(0);

                        //     vert.push_back(x - 0.5f);
                        //     vert.push_back(y - 0.5f);
                        //     vert.push_back(z - 0.5f);

                        //     vert.push_back(0);
                        //     vert.push_back(1);
                        // }
                    }
                }
            }
        }
        // Logger::Log(std::to_string(vert.size()), Logger::INFO);

        // GLuint VAO;
        // glGenVertexArrays(1, &VAO);
        // glBindVertexArray(VAO);

        // std::unique_ptr<Renderer::VertexBuffer> VBO(new Renderer::VertexBuffer());
        // VBO->Bind();
        // VBO->SetData(vert);

        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);

        // // glEnableVertexAttribArray(1);
        // // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));

        // glEnableVertexAttribArray(1);
        // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

        // // glBindVertexArray(0);

        // glDrawArrays(GL_TRIANGLES, 0, vert.size());
        Logger::Log("SKB\n", Logger::ERROR);

        glm::mat4 model;
        model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        std::unique_ptr<Renderer::Mesh> mesh = std::make_unique<Renderer::Mesh>(Renderer::Mesh(vert, indices, textbedr));
        Logger::Log("SKB2\n", Logger::ERROR);

        mesh->Draw(s);
        Logger::Log("SKB3\n", Logger::ERROR);

        //
        Logger::Log("SKB\n", Logger::ERROR);

        // for (int i = 0; i < pos.size(); i++)
        // {
        //     glm::mat4 model;
        //     model = glm::translate(glm::mat4(1.0f), pos[i]);
        //     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //     // if (i 16)
        //     cube.Draw(s);
        //     // else
        //     //   cubeBedrock.Draw(s);
        // }

        // mesh->Draw(s);

        // delete mesh;
        //  or (int i = 0; i < pos.size()/256; i++)
        //  {
        //      for (int j = 0; i < 256; i++)
        //      {
        //          if ( % 16)
        //          {
        //              glm::mat4 model;
        //              model = glm::translate(glm::mat4(1.0f), pos[i]);
        //              glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //              cube.Draw(s);
        //          }
        //      }
        //  }f

        // triangle.Draw(s);

        // glDepthFunc(GL_LEQUAL);

        // skyBoxShader.Use();

        // GLint viewLocSky = glGetUniformLocation(skyBoxShader.GetProgram(), "view");
        // GLint projLocSky = glGetUniformLocation(skyBoxShader.GetProgram(), "projection");

        // GLint texLocSky = glGetUniformLocation(skyBoxShader.GetProgram(), "skybox");

        // glUniformMatrix4fv(viewLocSky, 1, GL_FALSE, glm::value_ptr(view));
        //  glUniformMatrix4fv(projLocSky, 1, GL_FALSE, glm::value_ptr(projection));

        // glUniform1i(texLocSky, 0);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        // glBindVertexArray(skyboxVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // glBindVertexArray(0);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        // glDepthFunc(GL_LESS);
        //  const char *description;
        //  glfwGetError(&description);
        // Logger::Log(std::to_string(glGetError()), Logger::ERROR);
    }
    glfwTerminate();
}
