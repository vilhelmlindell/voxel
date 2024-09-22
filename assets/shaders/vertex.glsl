#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec2 TexCoord;

void main()
{
    //FragPos = vec3(model * vec4(position, 1.0));
    FragPos = position;
    gl_Position = projection * view * model * vec4(position, 1.0);

    vec3 absNormal = abs(normal);

    TexCoord = (absNormal.z * position.xy + absNormal.x * position.yz + absNormal.y * position.zx);
}
