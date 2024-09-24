#include "chunk.h"
#include "vertex.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stb/stb_image.h>

Chunk::Chunk(ChunkPool& chunk_pool) {
    for (size_t x = 0; x < WIDTH; ++x) {
        for (size_t y = 0; y < HEIGHT; ++y) {
            for (size_t z = 0; z < LENGTH; ++z) {
                BlockID block = BlockID::Grass;
                // Access the block at (x, y, z)
                blocks[x][y][z] = block; // Example: Set all blocks to Grass
            }
        }
    }
    init_buffers();
}

void Chunk::init_buffers() {
    // Generate and bind Vertex Array Object (VAO)
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind Vertex Buffer Object (VBO) for vertex data
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Define the vertex attribute pointers (position, normal, texture coordinates)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Generate and bind Shader Storage Buffer Object (SSBO) for block data
    glGenBuffers(1, &blocks_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, blocks_buffer);

    // Bind the SSBO to binding point 0 (must match the layout in your shader)
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, blocks_buffer);

    // Unbind VAO to prevent accidental modification
    glBindVertexArray(0);
}

void Chunk::generate_mesh() {
    for (size_t x = 0; x < WIDTH; x++) {
        for (size_t y = 0; y < HEIGHT; y++) {
            for (size_t z = 0; z < LENGTH; z++) {
                if (blocks[x][y][z] != BlockID::Empty) {
                    for (const Face& face : FACES) {
                        glm::ivec3 pos(x, y, z);
                        glm::ivec3 adjacent_pos = glm::ivec3(x, y, z) + FACE_VECTORS[(size_t)face];

                        if (is_outside_chunk(adjacent_pos) ||
                            get_block(adjacent_pos) == BlockID::Empty) {
                            generate_face(pos, face);
                        }
                    }
                }
            }
        }
    }

    // Ensure VAO and VBO are bound before updating buffer data
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(),
                 GL_STATIC_DRAW);

    // Bind blocks buffer and upload block data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, blocks_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(blocks), blocks, GL_STATIC_DRAW);

    // Optionally, unbind VAO and SSBO
    glBindVertexArray(0);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Chunk::generate_face(glm::ivec3 pos, Face face) {
    glm::vec3 normal = FACE_VECTORS[(size_t)face]; // Get the normal for the face

    Vertex face_vertices[4];

    // Iterate over the 4 vertices of the face
    for (int i = 0; i < 4; ++i) {
        const glm::ivec3& offset = FACE_VERTICES[(size_t)face][i];
        glm::ivec3 vertex_pos = glm::vec3(pos + offset);

        Vertex vertex;
        vertex.position = vertex_pos;
        vertex.normal = normal;
        face_vertices[i] = vertex;
    }
    vertices.push_back(face_vertices[0]);
    vertices.push_back(face_vertices[1]);
    vertices.push_back(face_vertices[2]);
    vertices.push_back(face_vertices[2]);
    vertices.push_back(face_vertices[3]);
    vertices.push_back(face_vertices[1]);
}

BlockID Chunk::get_block(glm::ivec3 pos) const { return blocks[pos.x][pos.y][pos.z]; }
void Chunk::set_block(glm::ivec3 pos, BlockID value) const {
    if (value == BlockID::Empty) {
    } else {
      for (Face face : FACES) {
        glm::ivec3 adjacent_pos = pos + FACE_VECTORS[(size_t)face];
      }
    }
}

bool Chunk::is_outside_chunk(size_t x, size_t y, size_t z) {
    return x < 0 || y < 0 || z < 0 || x >= WIDTH || y >= HEIGHT || z >= LENGTH;
}

bool Chunk::is_outside_chunk(glm::ivec3 pos) {
    return pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= WIDTH || pos.y >= HEIGHT ||
           pos.z >= LENGTH;
}
