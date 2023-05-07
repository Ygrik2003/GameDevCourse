#version 320 es
precision mediump float;

in vec4      v_position;
uniform vec3 triangle_color;

out vec4 frag_color;

void main()
{
    frag_color = vec4(triangle_color, 1.f);
}