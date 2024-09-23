#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

const vec3 NORMALS[6] = vec3[6](vec3(0.0, 1.0, 0.0), vec3(0.0, -1.0, 0.0), // Up, Down
                                vec3(1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), // Right, Left
                                vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, -1.0)); // Front, Back

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec2 TexCoord;

void main()
{
    FragPos = position;
    gl_Position = projection * view * model * vec4(position, 1.0);

    vec3 absNormal = abs(normal);

    TexCoord = (absNormal.z * position.xy + absNormal.x * position.yz + absNormal.y * position.zx);
}
