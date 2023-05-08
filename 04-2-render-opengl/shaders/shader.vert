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

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

uniform transformation_object tr_obj;
uniform transformation_camera tr_cam;

out vec4 v_position;
out vec4 v_color;

void main()
{

    v_position =
        vec4(a_position * tr_obj.rotate * tr_obj.scale + tr_obj.translate, 1.0);
    v_position = v_position * tr_cam.scale * tr_cam.translate * tr_cam.rotate *
                 tr_cam.projection;
    gl_Position = v_position;

    v_color = a_color;
}