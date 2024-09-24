#pragma once
#include "camera.h"
#include "chunk.h"
#include "shader.h"
#include <memory>
#include <queue>
#include <unordered_map>

namespace std {
    template <>
    struct hash<glm::ivec3> {
        size_t operator()(const glm::ivec3& v) const noexcept {
            size_t h1 = std::hash<int>()(v.x);
            size_t h2 = std::hash<int>()(v.y);
            size_t h3 = std::hash<int>()(v.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2); // Combine the hashes
        }
    };
}

class ChunkPool {
public:
    std::unordered_map<glm::ivec3, Chunk*> loaded_chunks;
    std::unique_ptr<Shader> shader;
    GLuint texture_array;

    ChunkPool(size_t max_chunk_count);
    ~ChunkPool();

    void init_graphics();

    void load_chunk(glm::ivec3 pos);
    void unload_chunk(glm::ivec3 pos);

    void render(const Camera& camera);

    Chunk* pool;
    std::queue<Chunk*> free_chunks;
};
