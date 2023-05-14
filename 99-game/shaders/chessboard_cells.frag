#version 320 es
precision mediump float;

const float ambient_strength = 0.1f;
const vec3  light_color      = vec3(1., 1., 1.);
const vec3  ambient          = ambient_strength * light_color;

in vec4           v_position;
in vec2           v_tex_coord;
in vec3           i_normal;
uniform sampler2D u_texture;

out vec4 o_color;

void main()
{
    vec4 color = texture2D(u_texture, v_tex_coord);
    o_color    = vec4(color.xyz, 1.);
}