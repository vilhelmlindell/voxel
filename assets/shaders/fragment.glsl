#version 460 core
layout(std430, binding = 0) buffer BlockData {
    uint blocks[];
};

uniform sampler2DArray blockTextures;
uniform ivec3 chunkSize;
uniform vec3 chunkPosition;

in vec3 FragPos;
in vec2 TexCoord;
out vec4 outColor;

void main() {
    // Calculate the block position within the chunk
    ivec3 blockPos = ivec3(FragPos);

    // Calculate 1D index from 3D position
    int index = blockPos.x + blockPos.y * chunkSize.x + blockPos.z * chunkSize.x * chunkSize.y8;

    // Get the block type
    uint blockType = blocks[index];

    // Use the block type to select the texture layer
    // Note: We cast to int as texture array indices must be integer
    vec4 texColor = texture(blockTextures, vec3(fract(TexCoord), blockType));

    outColor = texColor;

    // You can add additional lighting calculations here if needed
}
