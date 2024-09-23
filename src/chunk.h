#pragma once
#include "shader.h"
#include "vertex.h"
#include "camera.h"
#include <cstdint>
#include <glm/ext/vector_int3.hpp>
#include <glm/integer.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct ChunkPool;

enum class BlockID : uint64_t {
    Empty,
    Grass,
    Stone,
};

enum class Face : size_t { Up, Down, Right, Left, Front, Back };

const Face FACES[6] = {Face::Up, Face::Down, Face::Right, Face::Left, Face::Front, Face::Back};
const glm::ivec3 FACE_VECTORS[6] = {glm::ivec3(0, 1, 0), glm::ivec3(0, -1, 0),  // Up, Down
                                    glm::ivec3(1, 0, 0), glm::ivec3(-1, 0, 0),  // Right, Left
                                    glm::ivec3(0, 0, 1), glm::ivec3(0, 0, -1)}; // Front, Back
const glm::ivec3 FACE_VERTICES[6][4] = {
    {glm::ivec3(0, 1, 0), glm::ivec3(1, 1, 0), glm::ivec3(0, 1, 1), glm::ivec3(1, 1, 1)}, // Up
    {glm::ivec3(0, 0, 0), glm::ivec3(1, 0, 0), glm::ivec3(0, 0, 1), glm::ivec3(1, 0, 1)}, // Down
    {glm::ivec3(1, 0, 0), glm::ivec3(1, 1, 0), glm::ivec3(1, 0, 1),
     glm::ivec3(1, 1, 1)}, // Right (fixed)
    {glm::ivec3(0, 0, 0), glm::ivec3(0, 1, 0), glm::ivec3(0, 0, 1), glm::ivec3(0, 1, 1)}, // Left
    {glm::ivec3(0, 0, 1), glm::ivec3(1, 0, 1), glm::ivec3(0, 1, 1), glm::ivec3(1, 1, 1)}, // Front
    {glm::ivec3(0, 0, 0), glm::ivec3(1, 0, 0), glm::ivec3(0, 1, 0), glm::ivec3(1, 1, 0)}, // Back
};

using BitArray = std::uint32_t;
const size_t CHUNK_SIZE = sizeof(BitArray) * 8;

class Chunk {
  public:
    static const size_t WIDTH = CHUNK_SIZE;
    static const size_t HEIGHT = CHUNK_SIZE;
    static const size_t LENGTH = CHUNK_SIZE;

    BlockID blocks[WIDTH][HEIGHT][LENGTH];
    GLuint vbo, vao, ebo, blocks_buffer;
    std::vector<Vertex> vertices, indices;

    Chunk(ChunkPool& chunk_pool);

    void init_buffers();

    void generate_mesh();
    void generate_face(glm::ivec3 pos, Face face);

    BlockID& get_block(glm::ivec3);

    bool is_outside_chunk(size_t x, size_t y, size_t z);
    bool is_outside_chunk(glm::ivec3 pos);
};
