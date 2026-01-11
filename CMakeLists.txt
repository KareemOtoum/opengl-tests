cmake_minimum_required(VERSION 3.10)
project(OpenGLDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -g -O0")

add_subdirectory(external/glfw)

# Collect all .cpp files
file(GLOB SOURCES 
    src/water.cpp
    src/glad.c
    src/shader.cpp
    src/std_loader.cpp
    src/square.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)


# GLM
include(FetchContent)

FetchContent_Declare(
    glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
    
FetchContent_MakeAvailable(glm)
    
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm glfw)

# Put shaders in build folder
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
