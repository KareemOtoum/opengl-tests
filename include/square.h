#pragma once

#include <glm/glm.hpp>
#include "shader.h"
#include <vector>
#include <array>

class Square {
public:
  Square(glm::vec2 position, float width);
  ~Square();

  void update(float time);
  void draw();
  void set_position(glm::vec2 pos) { position_ = pos; }

private:
  glm::vec2 position_;
  glm::vec2 velocity_;
  glm::vec2 gravity_{0., -0.4};
  glm::vec2 water_push_{0., 0.05};
  float last_time{};
  const float water_level = -0.25;

  float width_;
  unsigned int VAO_;
  unsigned int VBO_;
  unsigned int EBO_;
  Shader shader_;
};