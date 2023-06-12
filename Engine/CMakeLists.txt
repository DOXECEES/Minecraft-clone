cmake_minimum_required(VERSION 3.25.0)

project(Engine)


add_subdirectory(dependencies/glfw)

set(ENGINE_SORCE_FILES  src/main.cpp
                        src/Window.cpp
                        src/VertexBuffer.cpp
                        )

set(ENGINE_HEADER_FILES src/Window.hpp
                        src/VertexBuffer.hpp
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
    )


target_link_directories(Engine 
    PRIVATE dependencies/glfw/src
    PRIVATE dependencies/glad/src)


target_link_libraries(Engine glfw)


