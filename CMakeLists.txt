cmake_minimum_required(VERSION 3.10)
project(OpenGLDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all .cpp files
file(GLOB SOURCES 
    src/main.cpp
    src/glad.c
    src/shader.cpp
    src/std_loader.cpp)

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

add_executable(${PROJECT_NAME} ${SOURCES})

# Find required packages
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

# Manually link GLFW (since you compiled from source)
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include include)
target_link_libraries(${PROJECT_NAME}
    /usr/local/lib/libglfw3.a
    OpenGL::GL
    X11::X11
    X11::Xrandr
    X11::Xinerama
    X11::Xi
    X11::Xcursor
    pthread
    dl
)
