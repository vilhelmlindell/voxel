#include "chunk_pool.h"
#include "glad/glad.h"
#include "program.h"
#include "texture.h"
#include <glm/gtc/type_ptr.hpp>

ChunkPool::ChunkPool(size_t max_chunk_count) {
    pool = static_cast<Chunk*>(malloc(max_chunk_count * sizeof(Chunk)));
    if (!pool) {
        throw std::bad_alloc();
    }
    for (size_t i = 0; i < max_chunk_count; i++) {
        free_chunks.push(&pool[i]);
    }

    init_graphics();
}

ChunkPool::~ChunkPool() { free(pool); }

void ChunkPool::init_graphics() {
    glEnable(GL_DEPTH_TEST);

    shader = std::make_unique<Shader>(WORKING_PATH / fs::path("assets/shaders/vertex.glsl"),
                                      WORKING_PATH / fs::path("assets/shaders/fragment.glsl"));

    texture_array = load_texture_array({WORKING_PATH / "assets/textures/dirt.png",
                                        WORKING_PATH / "assets/textures/stone_bricks.png"});

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);

    glUseProgram(shader->ID);

    glUniform1i(glGetUniformLocation(shader->ID, "textureArray"), 0);

    glm::ivec3 chunkSize(Chunk::WIDTH, Chunk::HEIGHT, Chunk::LENGTH);
    glUniform3iv(glGetUniformLocation(shader->ID, "chunkSize"), 1, glm::value_ptr(chunkSize));
}

void ChunkPool::load_chunk(glm::ivec3 pos) {
    if (free_chunks.empty()) {
        std::cerr << "Exceeded max chunk count, memory hasn't been allocated for more chunks"
                  << std::endl;
    }

    Chunk* chunk = free_chunks.front();
    free_chunks.pop();

    new (chunk) Chunk(*this);

    loaded_chunks.insert(std::make_pair(pos, chunk));

    chunk->generate_mesh();
}

void ChunkPool::unload_chunk(glm::ivec3 pos) {
    Chunk* memory = loaded_chunks[pos];
    free_chunks.push(memory);

    loaded_chunks.erase(pos);
}

void ChunkPool::render(const Camera& camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader->ID);

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE,
                       glm::value_ptr(camera.get_view_matrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE,
                       glm::value_ptr(camera.projection_matrix));

    GLint model_loc = glGetUniformLocation(shader->ID, "model");

    for (const auto& pair : loaded_chunks) {
        Chunk* chunk = pair.second;

        // Calculate the world position of the chunk
        glm::vec3 worldPos = pair.first * glm::ivec3(Chunk::WIDTH, Chunk::HEIGHT, Chunk::LENGTH);

        // Create and set the model matrix for this chunk
        glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPos);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, chunk->blocks_buffer);
        glBindVertexArray(chunk->vao);
        glDrawArrays(GL_TRIANGLES, 0, chunk->vertices.size());
    }
}
