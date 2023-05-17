#version 320 es
precision mediump float;

const vec3  light_color      = vec3(1., 1., 0.);
vec3        light_pos        = vec3(0., 2., 0.);
const float ambient_strength = 0.1f;
const vec3  ambient          = ambient_strength * light_color;

in vec3 v_position;
in vec2 v_tex_coord;
in vec3 i_normal;

in vec3           camera_pos;
uniform sampler2D u_texture;

out vec4 o_color;

void main()
{
    light_pos = vec3(-camera_pos.x, camera_pos.y, -camera_pos.z);

    vec4 color = texture2D(u_texture, v_tex_coord);

    vec3  light_dir = normalize(light_pos - v_position);
    float diff      = max(dot(i_normal, light_dir), 0.);
    vec3  diffuse   = diff * light_color;

    float specular_strength = 0.4f;
    vec3  view_dir          = normalize(camera_pos - v_position);
    vec3  reflect_dir       = reflect(-light_dir, i_normal);
    float spec              = pow(max(dot(view_dir, reflect_dir), 0.0), 128.);
    vec3  specular          = specular_strength * spec * light_color;

    vec3 dist = v_position - light_pos;

    vec3 result =
        (ambient + diffuse / max(1., dot(dist, dist)) + specular) * color.xyz;

    o_color = vec4(result, 1.);
}