#version 320 es
precision mediump float;

in vec4 v_position;
in vec4 v_color;

// uniform float i_time;

out vec4 o_color;

void main()
{
    if (gl_FrontFacing)
        o_color = v_color;
    else
        o_color = v_color.yxzw;
}
