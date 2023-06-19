#version 320 es

struct transformation_object
{
    mat3 rotate;
    vec3 scale;
    vec3 translate;
};
struct transformation_camera
{
    mat4 rotate;
    mat4 scale;
    mat4 translate;
    mat4 projection;
};

layout(location = 0) in vec3 a_position_1;
layout(location = 1) in vec4 a_color_1;

layout(location = 2) in vec3 a_position_2;
layout(location = 3) in vec4 a_color_2;

uniform transformation_object tr_obj;
uniform transformation_camera tr_cam;
uniform float                 i_time;
uniform float                 i_rand;
uniform float                 u_alpha;

out vec4 v_position;
out vec4 v_color;

void main()
{

    vec4 v_position_1;
    vec4 v_position_2;

    v_position_1 = vec4(
        a_position_1 * tr_obj.rotate * tr_obj.scale +
            /*vec3(5. * cos(i_rand), 5. * sin(i_rand), 0) +*/ tr_obj.translate,
        1.0);
    v_position_1 = v_position_1 * tr_cam.scale * tr_cam.translate *
                   tr_cam.rotate * tr_cam.projection;
    v_position_2 = vec4(
        a_position_2 * tr_obj.rotate * tr_obj.scale +
            /*vec3(5. * cos(i_rand), 5. * sin(i_rand), 0) +*/ tr_obj.translate,
        1.0);
    v_position_2 = v_position_2 * tr_cam.scale * tr_cam.translate *
                   tr_cam.rotate * tr_cam.projection;

    v_position = mix(v_position_1, v_position_2, u_alpha);

    gl_Position = v_position;

    v_color = mix(a_color_1, a_color_2, u_alpha);
}