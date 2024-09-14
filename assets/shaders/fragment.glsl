#version 330 core
out vec4 frag_color;
  
in vec2 tex_coord;

uniform sampler2D face_texture;

void main()
{
    frag_color = texture(face_texture, tex_coord);
}
