#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

out vec2 tex_coord_out;

void main()
{
    gl_Position = vec4(position, 1.0);
    tex_coord_out = tex_coord;
}
