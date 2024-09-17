#include "world.h"
#include "program.h"
#include "vertex.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

unsigned int load_texture(const std::filesystem::path& path) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0; // or handle the error accordingly
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}

template <size_t Width, size_t Height, size_t Length> World<Width, Height, Length>::World() {
    for (size_t x = 0; x < Width; ++x) {
        for (size_t y = 0; y < Height; ++y) {
            for (size_t z = 0; z < Length; ++z) {
                // Access the block at (x, y, z)
                blocks[x][y][z] = BlockID::Grass; // Example: Set all blocks to Grass
            }
        }
    }
}

template <size_t Width, size_t Height, size_t Length>
BlockID& World<Width, Height, Length>::operator[](const glm::ivec3& pos) {
    return blocks[pos.x][pos.y][pos.z];
}

template <size_t Width, size_t Height, size_t Length>
const BlockID& World<Width, Height, Length>::operator[](const glm::ivec3& pos) const {
    if (pos.x >= Width || pos.y >= Height || pos.z >= Length || pos.x < 0 || pos.y < 0 || pos.z < 0)
        return BlockID::Empty;

    return blocks[pos.x][pos.y][pos.z];
}

template <size_t Width, size_t Height, size_t Length>
void World<Width, Height, Length>::render(const Camera& camera) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE,
                       glm::value_ptr(glm::mat4(1.0f)));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE,
                       glm::value_ptr(camera.get_view_matrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));
    shader->use();
    glBindVertexArray(vao);
    // glDrawElements(GL_TRIANGLES, vertices.size() / 3, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

template <size_t Width, size_t Height, size_t Length>
void World<Width, Height, Length>::generate_mesh() {
  int count = 0;
    for (size_t x = 0; x < Width; ++x) {
        for (size_t y = 0; y < Height; ++y) {
            for (size_t z = 0; z < Length; ++z) {
                glm::ivec3 pos(x, y, z);
                if ((*this)[pos] != BlockID::Empty) {
                    for (const Face& face : FACES) {
                        glm::ivec3 adjacent_pos = glm::ivec3(x, y, z) + FACE_VECTORS[(size_t)face];
                        if ((*this)[adjacent_pos] == BlockID::Empty) {
                            generate_face(pos, face);
                            count++;
                        }
                    }
                }
            }
        }
    }
    std::cout << count << std::endl;

    shader = std::make_unique<Shader>(WORKING_PATH / fs::path("assets/shaders/vertex.glsl"),
                                      WORKING_PATH / fs::path("assets/shaders/fragment.glsl"));

    texture = load_texture(WORKING_PATH / fs::path("assets/images/container.jpg"));

    // Generate and bind Vertex Array Object and Vertex Buffer Object
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    // Bind and fill the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(),
                 GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (start at offset 3, as the first 3 are position)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates attribute (start at offset 6, after position + normal)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Optionally bind an Element Buffer Object (commented out in your code)
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    // GL_STATIC_DRAW);

    // Tell OpenGL which texture unit the sampler corresponds to (texture unit 0)
    glUniform1i(glGetUniformLocation(shader->ID, "face_texture"), 0);

    // Unbind the VAO (optional, for safety)
    glBindVertexArray(0);
}

template <size_t Width, size_t Height, size_t Length>
void World<Width, Height, Length>::generate_face(glm::ivec3 pos, Face face) {
    glm::vec3 normal = FACE_VECTORS[(size_t)face]; // Get the normal for the face

    // Texture coordinates (for each vertex of the face)
    const glm::vec2 texCoords[4] = {
        glm::vec2(0.0f, 0.0f), // bottom-left
        glm::vec2(1.0f, 0.0f), // bottom-right
        glm::vec2(0.0f, 1.0f), // top-left
        glm::vec2(1.0f, 1.0f)  // top-right
    };

    Vertex face_vertices[4];

    // Iterate over the 4 vertices of the face
    for (int i = 0; i < 4; ++i) {
        const glm::ivec3& offset = FACE_VERTICES[(size_t)face][i]; // Get vertex offset for the face
        glm::vec3 vertex_pos = glm::vec3(pos + offset);            // Calculate vertex position

        // Create a Vertex object with position, normal, and texCoord
        Vertex vertex;
        vertex.position = vertex_pos;    // Vertex position
        vertex.normal = normal;          // Face normal
        vertex.tex_coord = texCoords[i]; // Texture coordinate for the vertex
        face_vertices[i] = vertex;
    }
    vertices.push_back(face_vertices[0]);
    vertices.push_back(face_vertices[1]);
    vertices.push_back(face_vertices[2]);
    vertices.push_back(face_vertices[2]);
    vertices.push_back(face_vertices[3]);
    vertices.push_back(face_vertices[1]);
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

template class World<10, 10, 10>;
