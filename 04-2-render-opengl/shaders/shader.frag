#version 320 es
precision mediump float;
in vec4   v_position;
out vec4  frag_color;
// try main_one function name on linux mesa drivers
void main()
{
    if (v_position.z >= 0.0)
    {
        float light_green = 0.5 + v_position.z / 2.0;
        frag_color        = vec4(0.0, light_green, 0.0, 1.0);
    }
    else
    {
        float color = 0.5 - (v_position.z / -2.0);
        frag_color  = vec4(color, 0.0, 0.0, 1.0);
    }
}