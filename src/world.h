#pragma once
#include "shader.h"
#include "vertex.h"
#include "camera.h"
#include <array>
#include <glm/ext/vector_int3.hpp>
#include <glm/integer.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>

enum class BlockID {
    Empty,
    Grass,
};

enum class Face : size_t { Up, Down, Right, Left, Front, Back };

const Face FACES[6] = {Face::Up, Face::Down, Face::Right, Face::Left, Face::Front, Face::Back};
const glm::ivec3 FACE_VECTORS[6] = {glm::ivec3(0, 1, 0), glm::ivec3(0, -1, 0),  // Up, Down
                                    glm::ivec3(1, 0, 0), glm::ivec3(-1, 0, 0),  // Right, Left
                                    glm::ivec3(0, 0, 1), glm::ivec3(0, 0, -1)}; // Front, Back
const glm::ivec3 FACE_VERTICES[6][4] = {
    {glm::ivec3(0, 1, 0), glm::ivec3(1, 1, 0), glm::ivec3(0, 1, 1), glm::ivec3(1, 1, 1)}, // Up
    {glm::ivec3(0, 0, 0), glm::ivec3(1, 0, 0), glm::ivec3(0, 0, 1), glm::ivec3(1, 0, 1)}, // Down
    {glm::ivec3(1, 0, 0), glm::ivec3(1, 1, 0), glm::ivec3(1, 0, 1), glm::ivec3(1, 0, 1)}, // Right
    {glm::ivec3(0, 0, 0), glm::ivec3(0, 1, 0), glm::ivec3(0, 0, 1), glm::ivec3(0, 1, 1)}, // Left
    {glm::ivec3(0, 0, 1), glm::ivec3(1, 0, 1), glm::ivec3(0, 1, 1), glm::ivec3(1, 1, 1)}, // Front
    {glm::ivec3(0, 0, 0), glm::ivec3(1, 0, 0), glm::ivec3(0, 1, 0), glm::ivec3(1, 1, 0)}, // Back
};

template <size_t Width, size_t Height, size_t Length> class World {
  public:
    World();

    BlockID& operator[](const glm::ivec3& pos);
    const BlockID& operator[](const glm::ivec3& pos) const;

    void generate_mesh();
    void generate_face(glm::ivec3 pos, Face face);
    void render(const Camera& camera);

    size_t width();
    size_t height();
    size_t length();

  private:
    std::unique_ptr<Shader> shader;
    std::vector<Vertex> vertices;
    unsigned int vbo, vao, ebo, texture;
    std::array<std::array<std::array<BlockID, Length>, Height>, Width> blocks;
    // std::unordered_map<glm::vec3, unsigned int> indices_by_vertex;
};
