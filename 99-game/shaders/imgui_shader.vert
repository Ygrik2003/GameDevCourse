#version 300 es
precision mediump float;

layout(location = 0) in vec2 i_position;
layout(location = 2) in vec2 i_tex_coord;
layout(location = 3) in vec4 i_color;

out vec2 Frag_UV;
out vec4 Frag_Color;

void main()
{
    Frag_UV    = i_tex_coord;
    Frag_Color = i_color;
    // mat3 ProjMtx = P
    gl_Position = vec4(
        2. * vec3(i_position.x / 960., 1. - i_position.y / 540., 0.) - 1., 1.);
}