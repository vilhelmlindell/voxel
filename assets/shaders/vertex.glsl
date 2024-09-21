#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord_in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;

void main()
{
    // the multiplication is read from right to left
    FragPos = position;
    gl_Position = projection * view * model * vec4(position, 1.0);
}
