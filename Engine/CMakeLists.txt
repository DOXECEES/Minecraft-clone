cmake_minimum_required(VERSION 3.25.0)

project(Engine)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(dependencies/glfw)


find_package(OpenGL REQUIRED)

target_include_directories(Engine 
    PUBLIC OpenGL:GL
    PUBLIC dependencies/glfw/include
    PUBLIC dependencies/glad/include)


target_link_directories(Engine 
    PRIVATE dependencies/glfw/src
    PRIVATE dependencies/glad/src)


target_link_libraries(Engine glfw)
