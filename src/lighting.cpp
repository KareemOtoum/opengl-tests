#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"
#include "camera.h"

static constexpr int k_window_width { 1280 };
static constexpr int k_window_height { 720 };

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void render_triangle();

float deltaTime{};
float lastFrame{};

float lastX;
float lastY;

Camera camera(
    glm::vec3(0.0f, 0.0f,  3.0f),
    glm::vec3(0.0f, 1.0f,  0.0f),
    -90.0f,
    0
);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(k_window_width, k_window_height, "OpenGLDemo", NULL, NULL);

    glfwSetWindowFocusCallback(window,
    [](GLFWwindow*, int focused)
    {
        std::cout << "Focus: " << focused << '\n';
    });


    if(!window) {
        std::cout << "failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
    };
    
    unsigned int VAO, VBO, lightVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &lightVAO);
    
    glBindVertexArray(VAO);
    
    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Position attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // light VAO
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.f), 
    (float)k_window_width/(float)k_window_height,
    0.1f,
    100.f);
    
    // enable depth testing with z buffer (depth buffer)
    glEnable(GL_DEPTH_TEST);
    
    glm::vec3 cubePosition = glm::vec3( 0.0f,  0.0f,  0.0f);
    
    glfwSetCursorPosCallback(window, mouse_callback);  
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    if (glfwRawMouseMotionSupported())
    {
      glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    Shader lightingShader("shaders/cube.vs", "shaders/cube.fs");
    Shader lightSourceShader("shaders/light.vs", "shaders/light.fs");
    
    while(!glfwWindowShouldClose(window)) {
      float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        // input
        process_input(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

        lightingShader.use();

        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        
        glBindVertexArray(VAO);
        
        glm::mat4 model(1.0f);
        model = glm::translate(model, cubePosition);
        lightingShader.setMat4("model", model);
        
        // set colors
        lightingShader.setVec3("lightColor", glm::vec3(1., 1., 1.));
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // light source
        glBindVertexArray(lightVAO);
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        lightSourceShader.use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSourceShader.setMat4("model", model);
        lightSourceShader.setMat4("view", view);
        lightSourceShader.setMat4("projection", projection);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // check and call events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float camSpeed = 8 * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera::RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouseMovement {true};
    if(firstMouseMovement) {
        firstMouseMovement = false;
        lastX = xpos;
        lastY = ypos;
    }

    float xdiff = xpos - lastX;
    float ydiff = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xdiff, ydiff);
}

void render_triangle() {
    
}