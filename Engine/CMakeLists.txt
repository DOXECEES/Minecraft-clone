cmake_minimum_required(VERSION 3.25.0)

project(Engine)


add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glew)


set(ENGINE_SORCE_FILES  src/main.cpp
                        src/Render/Window.cpp
                        src/Render/VertexBuffer.cpp
                        src/Render/VertexArray.cpp
                        src/Render/Shader.cpp
                        )

set(ENGINE_HEADER_FILES src/Render/Window.hpp
                        src/Render/VertexBuffer.hpp
                        src/Render/VertexArray.hpp
                        src/Render/Shader.hpp
                        )

set(ENGINE_ALL_SOURCES ${ENGINE_HEADER_FILES}
                       ${ENGINE_SORCE_FILES}
                       )


add_executable(${PROJECT_NAME} ${ENGINE_ALL_SOURCES})


find_package(OpenGL REQUIRED)

target_include_directories(Engine 
    PUBLIC OpenGL:GL
    PUBLIC dependencies/glfw/include
    PUBLIC dependencies/glad/include
    PUBLIC dependencies/glm
    PUBLIC dependencies/glew/include()
    )


target_link_directories(Engine 
    PRIVATE dependencies/glfw/src
    PRIVATE dependencies/glad/src
    PRIVATE dependencies/glew/src
    )


target_link_libraries(Engine glfw glad)


