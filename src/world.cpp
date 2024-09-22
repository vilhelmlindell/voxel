#include "world.h"
#include "program.h"
#include "texture.h"
#include "vertex.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stb/stb_image.h>
template class World<CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE>;

template <size_t Width, size_t Height, size_t Length> 
World<Width, Height, Length>::World() {
    for (size_t x = 0; x < Width; ++x) {
        for (size_t y = 0; y < Height; ++y) {
            for (size_t z = 0; z < Length; ++z) {
                // Access the block at (x, y, z)
                blocks[x][y][z] = BlockID::Grass; // Example: Set all blocks to Grass
            }
        }
    }
}

// template <size_t Width, size_t Height, size_t Length>
// BlockID &World<Width, Height, Length>::operator[](const glm::ivec3 &pos) {
//   return blocks[pos.x][pos.y][pos.z];
// }

//template <size_t Width, size_t Height, size_t Length>
//const BlockID World<Width, Height, Length>::operator[](const glm::ivec3& pos) const {
//    if (pos.x >= Width || pos.y >= Height || pos.z >= Length || pos.x < 0 || pos.y < 0 || pos.z < 0)
//        return BlockID::Empty;
//
//    return blocks[pos.x][pos.y][pos.z];
//}

template <size_t Width, size_t Height, size_t Length>
void World<Width, Height, Length>::generate_mesh() {
    for (size_t x = 0; x < Width; x++) {
      for (size_t y = 0; y < Height; y++) {
        for (size_t z = 0; z < Length; z++) {
          glm::ivec3 pos(x, y, z);
          if (blocks[x][y][z] != BlockID::Empty) {
            for (const Face &face : FACES) {
              glm::ivec3 adjacent_pos =
                  glm::ivec3(x, y, z) + FACE_VECTORS[(size_t)face];
              if (blocks[adjacent_pos.x][adjacent_pos.y][adjacent_pos.z] == BlockID::Empty) {
                generate_face(pos, face);
              }
            }
          }
        }
      }
    }

    glEnable(GL_DEPTH_TEST);  

    shader = std::make_unique<Shader>(WORKING_PATH / fs::path("assets/shaders/vertex.glsl"),
                                      WORKING_PATH / fs::path("assets/shaders/fragment.glsl"));

    texture = load_texture(WORKING_PATH / fs::path("assets/textures/stone_bricks.png"));

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);

    shader->use();
    glUniform1i(glGetUniformLocation(shader->ID, "face_texture"), 0);

    GLuint bufferObject;
    glGenBuffers(1, &bufferObject);
    glBindBuffer(GL_TEXTURE_BUFFER, bufferObject);
    glBufferData(GL_TEXTURE_BUFFER, Width * Height * Length * sizeof(BlockID), blocks, GL_STATIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufferObject);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufferObject);
}

template <size_t Width, size_t Height, size_t Length>
void World<Width, Height, Length>::generate_face(glm::ivec3 pos, Face face) {
    glm::vec3 normal = FACE_VECTORS[(size_t)face]; // Get the normal for the face

    // Texture coordinates (for each vertex of the face)
    const glm::vec2 tex_coords[4] = {
        glm::vec2(0.0f, 1.0f), // bottom-left
        glm::vec2(1.0f, 1.0f), // bottom-right
        glm::vec2(0.0f, 0.0f), // top-left
        glm::vec2(1.0f, 0.0f), // top-right
    };

    Vertex face_vertices[4];

    // Iterate over the 4 vertices of the face
    for (int i = 0; i < 4; ++i) {
        const glm::ivec3& offset = FACE_VERTICES[(size_t)face][i];
        glm::ivec3 vertex_pos = glm::vec3(pos + offset);

        Vertex vertex;
        vertex.position = vertex_pos;
        vertex.normal = normal;
        vertex.tex_coord = tex_coords[i];
        face_vertices[i] = vertex;
    }
    vertices.push_back(face_vertices[0]);
    vertices.push_back(face_vertices[1]);
    vertices.push_back(face_vertices[2]);
    vertices.push_back(face_vertices[2]);
    vertices.push_back(face_vertices[3]);
    vertices.push_back(face_vertices[1]);
}

template <size_t Width, size_t Height, size_t Length>
void World<Width, Height, Length>::render(const Camera& camera) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader->use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE,
                       glm::value_ptr(glm::mat4(1.0f)));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE,
                       glm::value_ptr(camera.get_view_matrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

template <size_t Width, size_t Height, size_t Length> size_t World<Width, Height, Length>::width() {
    return Width;
}

template <size_t Width, size_t Height, size_t Length>
size_t World<Width, Height, Length>::height() {
    return Height;
}

template <size_t Width, size_t Height, size_t Length>
size_t World<Width, Height, Length>::length() {
    return Length;
}
