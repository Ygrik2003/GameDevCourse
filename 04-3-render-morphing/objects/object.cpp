#include "object.h"

#include <iostream>

triangle object::get_triangle(const uint32_t index)
{
    return triangle(_vertexes[indexes[3 * index + 0]],
                    _vertexes[indexes[3 * index + 1]],
                    _vertexes[indexes[3 * index + 2]]);
}

uint32_t object::size() const
{
    return count_triangles;
}

void object::set_rotate(float alpha, float beta, float gamma)
{
    tr_obj.rotate[0][0] = cos(alpha) * cos(beta);
    tr_obj.rotate[0][1] =
        cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma);
    tr_obj.rotate[0][2] =
        cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma);

    tr_obj.rotate[1][0] = sin(alpha) * cos(beta);
    tr_obj.rotate[1][1] =
        sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma);
    tr_obj.rotate[1][2] =
        sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma);

    tr_obj.rotate[2][0] = -sin(beta);
    tr_obj.rotate[2][1] = cos(beta) * sin(gamma);
    tr_obj.rotate[2][2] = cos(beta) * cos(gamma);
}
void object::set_translate(float dx, float dy, float dz)
{
    tr_obj.translate[0] = dx;
    tr_obj.translate[1] = dy;
    tr_obj.translate[2] = dz;
}

void object::set_scale(float scale_x, float scale_y, float scale_z)
{
    tr_obj.scale[0] = scale_x;
    tr_obj.scale[1] = scale_y;
    tr_obj.scale[2] = scale_z;
}
void object::set_discretization(float d_phi, float d_theta)
{
    d1 = d_phi;
    d2 = d_theta;
    calculate();
}

void object::render(engine* _engine, camera& _camera)
{
    for (uint32_t i = 0; i < size(); i++)
    {
        triangle tr = get_triangle(i);
        std::cout << 2 << std::endl;
        _engine->render_triangle(tr, tr, 1.f, tr_obj, _camera.get_transform());
    }
}
