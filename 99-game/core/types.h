#pragma once
#include <array>
#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI 3.141592653589793238
#endif
#include <cstddef>

struct uniform
{
    float alpha = 0.f; // For animation

    float width  = 0.f; // Resolution
    float height = 0.f;

    float* rotate_alpha_obj; // Rotate object
    float* rotate_beta_obj;
    float* rotate_gamma_obj;

    float* rotate_alpha_camera; // Rotate camera
    float* rotate_beta_camera;
    float* rotate_gamma_camera;

    float* translate_x_obj; // Translate object
    float* translate_y_obj;
    float* translate_z_obj;

    float* translate_x_camera; // Translate camera
    float* translate_y_camera;
    float* translate_z_camera;

    float* scale_x_obj; // Scale object
    float* scale_y_obj;
    float* scale_z_obj;
};

struct vertex;
struct vertex_colored;
struct vertex_textured;
struct vertex_colored_textured;

struct vector3d
{
    vector3d();
    vector3d(float x, float y, float z);
    vector3d(vertex v);

    vector3d  operator+(const vector3d& right);
    vector3d  operator+=(const vector3d& right);
    vector3d  operator-(const vector3d& right);
    vector3d& operator=(const vector3d& right);
    float     length() const;

    vector3d& normalize();

    float x = 0.;
    float y = 0.;
    float z = 0.;
};

namespace color
{
struct rgb
{
    rgb();
    rgb(float r, float g, float b);

    float r = 0.;
    float g = 0.;
    float b = 0.;
};

struct rgba : rgb
{
    rgba();
    rgba(float r, float g, float b, float a);

    float a = 1.;
};
} // namespace color

struct vertex
{
    vertex();
    vertex(float x, float y, float z);

    float x = 0.;
    float y = 0.;
    float z = 0.;

    vector3d normal;
};

struct vertex_colored : vertex
{
    vertex_colored() {}
    vertex_colored(vertex ver, color::rgba color);

    color::rgba rgba;
};

struct vertex_textured : vertex
{
    vertex_textured();
    vertex_textured(vertex ver, float u, float v);

    float u = 0.;
    float v = 0.;
};

struct vertex_colored_textured : vertex_colored
{
    vertex_colored_textured();
    vertex_colored_textured(vertex ver, color::rgba clr, float u, float v);

    float u = 0.;
    float v = 0.;
};

inline float dot(const vector3d& x, const vector3d& y)
{
    return x.x * y.x + x.y * y.y + x.z * y.z;
}

inline vector3d cross(const vector3d& x, const vector3d& y)
{
    return vector3d(
        x.y * y.z - y.y * x.z, x.z * y.x - y.z * x.x, x.x * y.y - y.x * x.y);
}

template <class T>
struct triangle
{
    triangle(){};
    triangle(T v0, T v1, T v2)
    {
        vertexes[0] = v0;
        vertexes[1] = v1;
        vertexes[2] = v2;
        calc_normal();
    }
    triangle(T v0, T v1, T v2, vector3d norm)
    {
        vertexes[0] = v0;
        vertexes[1] = v1;
        vertexes[2] = v2;
        normal      = norm;
    }

    void calc_normal()
    {
        normal = cross(vector3d(vertexes[2]) - vector3d(vertexes[0]),
                       vector3d(vertexes[1]) - vector3d(vertexes[0]))
                     .normalize();
    }

    T& operator[](const size_t index)
    {
        assert(index >= 3);
        return vertexes[index];
    }

    T*   data() { return vertexes.data(); }
    auto begin() { return vertexes.begin(); }
    auto end() { return vertexes.end(); }

    std::array<T, 3> vertexes;
    vector3d         normal;
};