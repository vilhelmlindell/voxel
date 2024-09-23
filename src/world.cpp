#include "chunk.h"
#include "program.h"
#include "texture.h"
#include "vertex.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stb/stb_image.h>

Chunk::Chunk() {
    for (size_t x = 0; x < WIDTH; ++x) {
        for (size_t y = 0; y < HEIGHT; ++y) {
            for (size_t z = 0; z < LENGTH; ++z) {
                BlockID block = BlockID::Grass;
                if (x % 2 == 0 || y % 2 == 0 || z % 2 == 0)
                  block = BlockID::Stone;
                // Access the block at (x, y, z)
                blocks[x][y][z] = block; // Example: Set all blocks to Grass
            }
        }
    }
}

void Chunk::generate_mesh() {
    for (size_t x = 0; x < WIDTH; x++) {
        for (size_t y = 0; y < HEIGHT; y++) {
            for (size_t z = 0; z < LENGTH; z++) {
                if (blocks[x][y][z] != BlockID::Empty) {
                    for (const Face& face : FACES) {
                        glm::ivec3 pos(x, y, z);
                        glm::ivec3 adjacent_pos = glm::ivec3(x, y, z) + FACE_VECTORS[(size_t)face];

                        // if (is_outside_chunk(adjacent_pos)) {
                        //   continue;
                        // }

                        // std::cout << glm::to_string(adjacent_pos) << std::endl;

                        if (is_outside_chunk(adjacent_pos) ||
                            get_block(adjacent_pos) == BlockID::Empty) {
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

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(),
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);

    shader->use();

    glm::ivec3 chunkSize(WIDTH, HEIGHT, LENGTH);
    GLint chunkSizeLocation = glGetUniformLocation(shader->ID, "chunkSize");
    glUniform3iv(chunkSizeLocation, 1, glm::value_ptr(chunkSize));

    GLuint bufferObject;
    glGenBuffers(1, &bufferObject);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferObject); // Change to GL_SHADER_STORAGE_BUFFER
    glBufferData(GL_SHADER_STORAGE_BUFFER, WIDTH * HEIGHT * LENGTH * sizeof(BlockID), blocks,
                 GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufferObject);

    GLuint textureArrayID = load_texture_array({WORKING_PATH / "assets/textures/stone_bricks.png",
                                                WORKING_PATH / "assets/textures/dirt.png"});
    ;
    GLuint textureUnit = 0;

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);

    glUseProgram(shader->ID);
    GLint texArrayUniform = glGetUniformLocation(shader->ID, "textureArray");
    glUniform1i(texArrayUniform, textureUnit);
}

bool Chunk::is_outside_chunk(size_t x, size_t y, size_t z) {
    return x < 0 || y < 0 || z < 0 || x >= WIDTH || y >= HEIGHT || z >= LENGTH;
}

bool Chunk::is_outside_chunk(glm::ivec3 pos) {
    return pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= WIDTH || pos.y >= HEIGHT ||
           pos.z >= LENGTH;
}

BlockID& Chunk::get_block(glm::ivec3 pos) {
    return blocks[pos.x][pos.y][pos.z];
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

void Chunk::render(const Camera& camera) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    //glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10000.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE,
                       glm::value_ptr(glm::mat4(1.0f)));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE,
                       glm::value_ptr(camera.get_view_matrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE,
                       glm::value_ptr(camera.projection_matrix));

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

size_t Chunk::width() {
    return WIDTH;
}

size_t Chunk::height() {
    return HEIGHT;
}

size_t Chunk::length() {
    return LENGTH;
}
