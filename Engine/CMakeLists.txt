cmake_minimum_required(VERSION 3.25.0)

project(Engine)
set (CMAKE_CXX_STANDARD 20)

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glad)


set(ENGINE_SORCE_FILES  src/main.cpp
                        src/Render/Window.cpp
                        src/Render/VertexBuffer.cpp
                        src/Render/VertexArray.cpp
                        src/Render/Shader.cpp
                        src/Render/IndexBuffer.cpp
                        src/Render/Texture.cpp
                        src/Render/Mesh.cpp
                        src/EventsHandler/Keyboard.cpp
                        src/EventsHandler/Mouse.cpp
                        src/EventsHandler/Input.cpp
                        src/Logger.cpp
                        src/Camera.cpp
                        dependencies/lodepng/lodepng.cpp

                        )

set(ENGINE_HEADER_FILES src/Render/Window.hpp
                        src/Render/VertexBuffer.hpp
                        src/Render/VertexArray.hpp
                        src/Render/Shader.hpp
                        src/Render/IndexBuffer.hpp
                        src/Render/Texture.hpp
                        src/Render/Mesh.hpp
                        src/EventsHandler/Keyboard.hpp
                        src/EventsHandler/Mouse.hpp
                        src/EventsHandler/Input.hpp
                        src/Logger.hpp
                        src/Camera.hpp
                        src/Templates/Array3D.hpp
                        dependencies/lodepng/lodepng.h
                        
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

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)                   