#include "types.h"

color::rgb::rgb() {}
color::rgb::rgb(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

vertex::vertex() {}
vertex::vertex(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

vertex_textured::vertex_textured() {}
vertex_textured::vertex_textured(vertex ver, float u, float v)
{
    x       = ver.x;
    y       = ver.y;
    z       = ver.z;
    this->u = u;
    this->v = v;
}

vector3d::vector3d() {}
vector3d::vector3d(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
};

vector3d::vector3d(vertex v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

vector3d vector3d::operator+(const vector3d& right)
{
    return vector3d(this->x + right.x, this->y + right.y, this->z + right.z);
}
vector3d vector3d::operator+=(const vector3d& right)
{
    this->x += right.x;
    this->y += right.y;
    this->z += right.z;
    return *this;
}
vector3d vector3d::operator-(const vector3d& right)
{
    return vector3d(this->x - right.x, this->y - right.y, this->z - right.z);
}

vector3d& vector3d::operator=(const vector3d& right)
{
    this->x = right.x;
    this->y = right.y;
    this->z = right.z;
    return *this;
}

float vector3d::length() const
{
    return std::pow(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2), 1. / 2.);
}

vector3d& vector3d::normalize()
{
    float len = length();
    x /= len;
    y /= len;
    z /= len;
    return *this;
}

vertex_colored::vertex_colored(vertex ver, color::rgb color)
{
    x   = ver.x;
    y   = ver.y;
    z   = ver.z;
    rgb = color;
}