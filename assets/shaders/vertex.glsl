#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord_in;

out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // the multiplication is read from right to left
    gl_Position = projection * view * model * vec4(position, 1.0);
    tex_coord = tex_coord_in;
}
