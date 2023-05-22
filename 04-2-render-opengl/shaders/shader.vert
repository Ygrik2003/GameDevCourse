#version 200 es

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

in vec3 a_position;
in vec4 a_color;

uniform transformation_object tr_obj;
uniform transformation_camera tr_cam;
uniform float                 i_time;
uniform float                 i_rand;

out vec4 v_position;
out vec4 v_color;

void main()
{

    v_position =
        vec4(a_position * tr_obj.rotate * tr_obj.scale +
                 0.1 * vec3(cos(i_rand), sin(i_rand), 0) + tr_obj.translate,
             1.0);
    v_position = v_position * tr_cam.scale * tr_cam.translate * tr_cam.rotate *
                 tr_cam.projection;
    gl_Position = v_position;

    v_color = a_color;
}