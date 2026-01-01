cmake_minimum_required(VERSION 3.10)
project(OpenGLDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all .cpp files
file(GLOB SOURCES 
    src/water.cpp
    src/glad.c
    src/shader.cpp
    src/std_loader.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Manually link GLFW (since you compiled from source)
target_include_directories(${PROJECT_NAME} PRIVATE /usr/local/include include)

add_subdirectory(external/glfw)
include_directories(external/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw)

# System libraries
if(WIN32) 
    target_link_libraries(${PROJECT_NAME} opengl32)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} GL X11 Xrandr Xi pthread dl)
endif()