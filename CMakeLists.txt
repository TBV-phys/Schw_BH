cmake_minimum_required(VERSION 3.16)

project(Null_SchwBH LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(GL_SILENCE_DEPRECATION)

# Executables

set(SOURCES
    src/main.cpp

    src/physics/RK4.cpp
    src/physics/RK4.h
    src/physics/physicsBH.cpp
    src/physics/physicsBH.h

    src/render/draws.cpp
    src/render/draws.h
)

add_executable(Null_SchwBH ${SOURCES})

source_group(TREE ${CMAKE_SOURCE_DIR}/src FILES ${SOURCES})

# Headers
target_include_directories(Null_SchwBH PRIVATE
    src
    src/physics
    src/render
    ext/glfw/3.4/include
)

# GLFW local
target_link_directories(Null_SchwBH PRIVATE
    ext/glfw/3.4/lib
)

# OpenGL de macOS
find_package(OpenGL REQUIRED)

target_link_libraries(Null_SchwBH PRIVATE
    glfw
    OpenGL::GL
)
