#version 320 es

#define M_PI 3.1415926535897932384626433832795

struct uniforms
{
    float alpha; // For animation

    float width; // Resolution
    float height;

    float rotate_x_obj; // Rotate object
    float rotate_y_obj;
    float rotate_z_obj;

    float rotate_x_camera; // Rotate camera
    float rotate_y_camera;
    float rotate_z_camera;

    float translate_x_obj; // Translate object
    float translate_y_obj;
    float translate_z_obj;

    float translate_x_camera; // Translate camera
    float translate_y_camera;
    float translate_z_camera;

    float scale_x_obj; // Scale object
    float scale_y_obj;
    float scale_z_obj;
};

layout(location = 0) in vec3 i_position;
in vec2 i_tex_coord;

out vec4 v_position;
out vec2 v_tex_coord;

uniform uniforms u_uniforms;

const float front = 0.1f;
const float back  = 5.f;
const float fovy  = M_PI / 2.;

void main()
{
    // clang-format off
    // mat4 view = mat4(

    // );
    // clang-format on

    // mat4 projection = glm::projection(
    //     fovy, u_uniforms.width / u_uniforms.height, front, back);

    v_tex_coord = i_tex_coord / 8.;
    v_position  = vec4(i_position.x / (u_uniforms.width / u_uniforms.height),
                      i_position.y,
                      i_position.z,
                      1.);
    gl_Position = v_position;
}