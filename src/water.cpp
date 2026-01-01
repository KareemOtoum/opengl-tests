#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "shader.h"
#include "stb_image.h"
#include "window.h"
#include <vector>
#include <iostream>

static constexpr int k_window_width{800};
static constexpr int k_window_height{600};

static Window window(k_window_width, k_window_height, "OpenGL Test");

auto make_quad_vertices(int n) -> std::vector<float>;
auto make_quad_indices(int n) -> std::vector<unsigned int>;

int main()
{
  glfwMakeContextCurrent(window.get_window());

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
  }

  const int n = 100;
  auto vertices = make_quad_vertices(n);
  auto indices = make_quad_indices(n);

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  // setup vbo
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  // SETUP EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
    indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  Shader shader("../src/vertex.vs", "../src/fragment.fs");

  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glDepthMask(GL_FALSE); // don't write depth
  // draw transparent objects LAST

  // let vertex shader determine point size
  // glEnable(GL_PROGRAM_POINT_SIZE);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  while(!glfwWindowShouldClose(window.get_window())) {
    glClearColor(0.14, 0.11, 0.12, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);  

    shader.use();

    int resolutionLocation = glGetUniformLocation(shader.m_id, "resolution");
    glUniform2f(resolutionLocation, k_window_width, k_window_height); 
    
    int timeLocation = glGetUniformLocation(shader.m_id, "time");
    glUniform1f(timeLocation, glfwGetTime()); 
    glBindVertexArray(VAO);
    //glDrawArrays(GL_POINTS, 0, n * n);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window.get_window());
    glfwPollEvents();
  } 
}

auto make_quad_vertices(int n) -> std::vector<float> {
    std::vector<float> vertices;
    float vertex_spacing = 2.0f / (n - 1); // so last vertex reaches x=1

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            float x = -1.0f + c * vertex_spacing;
            float y = -0.3f + -r * vertex_spacing; // bottom half
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }
    }

    return vertices;
}

auto make_quad_indices(int n) -> std::vector<unsigned int> {
    std::vector<unsigned int> indices;

    for (int r = 0; r < n - 1; ++r) {
        for (int c = 0; c < n - 1; ++c) {
            int v0 = r * n + c;
            int v1 = v0 + 1;
            int v2 = v0 + n;
            int v3 = v2 + 1;

            // triangle 1
            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);

            // triangle 2
            indices.push_back(v2);
            indices.push_back(v1);
            indices.push_back(v3);
        }
    }

    return indices;
}
