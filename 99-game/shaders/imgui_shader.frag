#version 300 es
precision mediump float;

in vec2 Frag_UV;
in vec4 Frag_Color;

uniform sampler2D u_texture;

out vec4 o_color;

vec4 texture2D(sampler2D tex, vec2 uv);

void main()
{
    o_color = Frag_Color * texture2D(u_texture, Frag_UV);
}
