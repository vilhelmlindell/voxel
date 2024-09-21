#version 430
#extension GL_EXT_shader_explicit_arithmetic_types : enable

// Block data buffer
layout(std430, binding = 0) buffer BlockData {
    uint16_t blocks[];
};

// Texture array sampler
uniform sampler2DArray blockTextures;

// Input from vertex shader
in vec3 fragTexCoord;
in vec3 fragPosition;

// Output color
out vec4 outColor;

// Uniforms for chunk information
uniform ivec3 chunkSize;
uniform vec3 chunkPosition;

void main() {
    // Calculate the block position within the chunk
    ivec3 blockPos = ivec3(fragPosition - chunkPosition);
    
    // Calculate 1D index from 3D position
    int index = blockPos.x + blockPos.y * chunkSize.x + blockPos.z * chunkSize.x * chunkSize.y;
    
    // Get the block type
    uint16_t blockType = blocks[index];
    
    // Use the block type to select the texture layer
    // Note: We cast to int as texture array indices must be integer
    vec4 texColor = texture(blockTextures, vec3(fragTexCoord.xy, int(blockType)));
    
    outColor = texColor;
    // You can add additional lighting calculations here if needed
}
