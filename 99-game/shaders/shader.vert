#version 300 es

struct uniforms
{
    float alpha; // For animation

    float width; // Resolution
    float height;

    float rotate_alpha_obj; // Rotate object
    float rotate_beta_obj;
    float rotate_gamma_obj;

    float rotate_alpha_camera; // Rotate camera
    float rotate_beta_camera;
    float rotate_gamma_camera;

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

in vec3 i_position;
in vec3 i_normal;
in vec2 i_tex_coord;
in vec4 i_color;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_tex_coord;
out vec3 camera_pos;

uniform uniforms u_uniforms;
uniform vec3     u_normal;

const float front = 0.0001f;
const float back  = 30.f;
float       fovy  = 3.14159 / 2.;

mat4 rotate_matrix(float alpha, float beta, float gamma)
{
    return mat4(
        vec4(cos(alpha) * cos(beta), sin(alpha) * cos(beta), -sin(beta), 0),
        vec4(cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma),
             sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma),
             cos(beta) * sin(gamma),
             0),
        vec4(cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma),
             sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma),
             cos(beta) * cos(gamma),
             0),
        vec4(0., 0., 0., 1.));
}

mat4 translate_matrix(float dx, float dy, float dz)
{
    return mat4(vec4(1., 0., 0., dx),
                vec4(0., 1., 0., dy),
                vec4(0., 0., 1., dz),
                vec4(0., 0., 0., 1.));
}

mat4 scale_matrix(float sx, float sy, float sz)
{
    return mat4(vec4(sx, 0., 0., 0.),
                vec4(0., sy, 0., 0.),
                vec4(0., 0., sz, 0.),
                vec4(0., 0., 0., 1.));
}

mat4 perspective_matrix(float fovy, float aspect, float front, float back)
{
    return mat4(vec4(1. / (aspect * tan(fovy / 2.)), 0., 0., 0.),
                vec4(0., 1. / tan(fovy / 2.), 0., 0.),
                vec4(0.,
                     0.,
                     (back + front) / (back - front),
                     -2. * back * front / (back - front)),
                vec4(0., 0., 1., 0.));
}

void main()
{

    mat4 projection = perspective_matrix(
        fovy, u_uniforms.width / u_uniforms.height, front, back);

    v_tex_coord = i_tex_coord;
    camera_pos  = vec3(-u_uniforms.translate_x_camera,
                      -u_uniforms.translate_y_camera,
                      -u_uniforms.translate_z_camera);

    mat4 model = scale_matrix(u_uniforms.scale_x_obj,
                              u_uniforms.scale_y_obj,
                              u_uniforms.scale_z_obj) *
                 translate_matrix(u_uniforms.translate_x_obj,
                                  u_uniforms.translate_y_obj,
                                  u_uniforms.translate_z_obj) *
                 rotate_matrix(u_uniforms.rotate_alpha_obj,
                               u_uniforms.rotate_beta_obj,
                               u_uniforms.rotate_gamma_obj);

    v_position = vec3(vec4(i_position, 1.) * model);

    v_normal = normalize((vec4(i_normal, 0.f) * model).xyz);

    gl_Position = vec4(v_position, 1.) *
                  translate_matrix(u_uniforms.translate_x_camera,
                                   u_uniforms.translate_y_camera,
                                   u_uniforms.translate_z_camera) *
                  rotate_matrix(u_uniforms.rotate_alpha_camera,
                                u_uniforms.rotate_beta_camera,
                                u_uniforms.rotate_gamma_camera) *
                  projection;
}