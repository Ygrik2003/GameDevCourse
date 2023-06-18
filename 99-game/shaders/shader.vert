#version 300 es

struct uniforms
{
    float alpha;            // For animation
    vec2  size_window;      // Resolution
    vec3  rotate_obj;       // Rotate object
    vec3  rotate_camera;    // Rotate camera
    vec3  translate_obj;    // Translate object
    vec3  translate_camera; // Translate camera
    vec3  scale_obj;        // Scale object
};

in vec3 i_position;
in vec3 i_normal;
in vec2 i_tex_coord;
in vec4 i_color;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_tex_coord;
out vec3 camera_pos;

uniform vec3     u_normal;
uniform uniforms u_uniforms;

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
        fovy, u_uniforms.size_window.x / u_uniforms.size_window.y, front, back);

    v_tex_coord = i_tex_coord;
    camera_pos  = vec3(-u_uniforms.translate_camera.x,
                      -u_uniforms.translate_camera.y,
                      -u_uniforms.translate_camera.z);

    mat4 model = scale_matrix(u_uniforms.scale_obj.x,
                              u_uniforms.scale_obj.y,
                              u_uniforms.scale_obj.z) *
                 translate_matrix(u_uniforms.translate_obj.x,
                                  u_uniforms.translate_obj.y,
                                  u_uniforms.translate_obj.z) *
                 rotate_matrix(u_uniforms.rotate_obj.x,
                               u_uniforms.rotate_obj.y,
                               u_uniforms.rotate_obj.z);

    v_position = vec3(vec4(i_position, 1.) * model);

    v_normal = normalize((vec4(i_normal, 0.f) * model).xyz);

    gl_Position = vec4(v_position, 1.) *
                  translate_matrix(u_uniforms.translate_camera.x,
                                   u_uniforms.translate_camera.y,
                                   u_uniforms.translate_camera.z) *
                  rotate_matrix(u_uniforms.rotate_camera.x,
                                u_uniforms.rotate_camera.y,
                                u_uniforms.rotate_camera.z) *
                  projection;
}