#include "square.h"

Square::Square(glm::vec2 position, float width) 
 : position_{position},
   width_{width},
   shader_{
    "shaders/square.vs", 
    "shaders/square.fs"
    } {
  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);
}

void Square::update(float time) {
  float dt = time - last_time;

  if(position_.y <= water_level) {
    water_push_ *= 1.01;
  } else {
    water_push_ *= 0.95;
    if(water_push_.y < 0.1) {
      water_push_.y = 0.1;
    }
  }

  position_ += water_push_ * dt + gravity_ * dt;
  last_time = time;
}

void Square::draw() {
  constexpr int vertex_count { 4 };
  std::array<float, vertex_count * 3> vertices{};
  constexpr int indices_count { 6 };
  std::array<unsigned int, indices_count> indices{};

  // gen vertex data
  int i{};
  vertices[i++] = position_.x - width_ / 2;
  vertices[i++] = position_.y + width_ / 2;
  i++;

  vertices[i++] = position_.x + width_ / 2;
  vertices[i++] = position_.y + width_ / 2;
  i++;
  
  vertices[i++] = position_.x - width_ / 2;
  vertices[i++] = position_.y - width_ / 2;
  i++;

  vertices[i++] = position_.x + width_ / 2;
  vertices[i++] = position_.y - width_ / 2;

  i = 0;
  indices[i++] = 0; indices[i++] = 1; indices[i++] = 2;
  indices[i++] = 1; indices[i++] = 3; indices[i++] = 2;
  
  glBindVertexArray(VAO_);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
    vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
    indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
    3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  shader_.use();
  glDrawElements(GL_TRIANGLES, indices.size(),
    GL_UNSIGNED_INT, 0);
}

Square::~Square() {
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
  glDeleteBuffers(1, &EBO_);
}