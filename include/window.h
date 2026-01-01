#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

class Window {

public:
  Window(int width, int height, std::string_view title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    
    window_ = glfwCreateWindow(width, height, title.data(), NULL, NULL);

    if(!window_) {
      std::cout << "failed to create window\n";
      glfwTerminate();
    }

    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
  }

  GLFWwindow* get_window() { return window_; }

  ~Window() {
    glfwTerminate();
  }
private:
  GLFWwindow* window_;
};

 