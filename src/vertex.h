#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// struct Vertex {
//   glm::vec3 position;
//   glm::vec3 normal;
//   glm::vec2 tex_coord;
// };
struct Vertex {
  float position[3];
  float normal[3];
  float tex_coord[2];
} __attribute__((__packed__));
