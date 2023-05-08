#version 320 es
precision mediump float;

in vec4 v_position;
in vec4 v_color;

out vec4 o_color;

void main()
{
    o_color = v_color;
}
