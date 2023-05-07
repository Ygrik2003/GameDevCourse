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

in vec3 a_position;
layout(location = 0) uniform transformation_object tr_obj;
// uniform transformation_camera tr_cam;

out vec4 v_position;

void main()
{

    v_position = vec4(a_position * tr_obj.rotate + tr_obj.translate, 1.0);
    v_position.x *= 9. / 16.;
    gl_Position = v_position;
}