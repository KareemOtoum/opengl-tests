#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "shader.h"
#include "stb_image.h"
#include "window.h"

static constexpr int k_window_width{800};
static constexpr int k_window_height{600};

static Window window(k_window_width, k_window_height, "OpenGL Test");

int main()
{
  glfwMakeContextCurrent(window.get_window());

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
  }

  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f
  };

  unsigned int indices[] = {
    0, 2, 1,
    1, 2, 3
  };

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // setup vbo
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  // setup ebo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  Shader shader("../src/vertex.vs", "../src/fragment.fs");

  while(!glfwWindowShouldClose(window.get_window())) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int ourColorLocation = glGetUniformLocation(shader.m_id, "ourColor");
    shader.use();
    float timeVal = glfwGetTime();
    float orangeVal = sin(timeVal) / 2 + 0.5f;
    glUniform4f(ourColorLocation, orangeVal, 0.5f, 0.0f, 1.0f);
    
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window.get_window());
    glfwPollEvents();
  } 
}