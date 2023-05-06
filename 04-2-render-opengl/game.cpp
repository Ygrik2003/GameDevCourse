#include "game.h"
#include <cassert>

sphere::sphere(uint r)
{
    int    index = 0;
    double theta = 0, phi;

    _vertexes.push_back(get_sphere(0, 0, r));
    for (int theta_counter = 1; theta_counter <= M_PI / d2; theta_counter++)
    {
        phi = 0;
        for (int phi_counter = 0; phi_counter <= 2 * M_PI / d1; phi_counter++)
        {
            _vertexes.push_back(get_sphere(phi, theta, r));

            if (theta_counter < static_cast<int>(M_PI / d2))
            {
                indexes.push_back(get_index(phi_counter, theta_counter));
                indexes.push_back(get_index(phi_counter + 1, theta_counter));
                indexes.push_back(
                    get_index(phi_counter + 1, theta_counter + 1));

                indexes.push_back(get_index(phi_counter, theta_counter));
                indexes.push_back(get_index(phi_counter, theta_counter + 1));
                indexes.push_back(
                    get_index(phi_counter + 1, theta_counter + 1));
            }
            else
            {
                indexes.push_back(get_index(phi_counter, theta_counter));
                indexes.push_back(get_index(phi_counter + 1, theta_counter));
                indexes.push_back(
                    get_index(phi_counter + 1, theta_counter + 1));
            }
            phi += d1;
        }
        theta += d2;
    }
    _vertexes.push_back(get_sphere(0, M_PI, r));
    count_triangles = indexes.size() / 3;
}

triangle object::get_triangle(const uint index)
{
    return triangle(_vertexes[indexes[3 * index + 0]],
                    _vertexes[indexes[3 * index + 1]],
                    _vertexes[indexes[3 * index + 2]]);
}

uint object::size() const
{
    return count_triangles;
}

uint sphere::get_index(int phi_c, int theta_c)
{
    if (theta_c == 0)
    {
        return 0;
    }
    else if (theta_c == static_cast<int>(M_PI / d2) + 1)
    {
        return (static_cast<int>(M_PI / d2) + 1) *
                   (static_cast<int>(2 * M_PI / d1)) +
               1;
    }
    else
    {
        return 1 + (theta_c - 1) * static_cast<int>(2 * M_PI / d1 + 1) +
               phi_c % static_cast<int>(2 * M_PI / d1);
    }
};
vertex sphere::get_sphere(double phi, double theta, uint r)
{

    return vertex{ static_cast<float>(r * (cos(phi) * sin(theta))),
                   static_cast<float>(r * (sin(phi) * sin(theta))),
                   static_cast<float>(r * (cos(theta))) };
}

void object::set_rotate(float alpha, float beta, float gamma)
{
    mat_rotate[0][0] = cos(alpha) * cos(beta);
    mat_rotate[0][1] =
        cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma);
    mat_rotate[0][2] =
        cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma);

    mat_rotate[1][0] = sin(alpha) * cos(beta);
    mat_rotate[1][1] =
        sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma);
    mat_rotate[1][2] =
        sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma);

    mat_rotate[2][0] = -sin(beta);
    mat_rotate[2][1] = cos(beta) * sin(gamma);
    mat_rotate[2][2] = cos(beta) * cos(gamma);

    mat_rotate[3][3] = 1;
}
void object::set_translate(float dx, float dy, float dz)
{
    mat_translate[3][0] = dx;
    mat_translate[3][1] = dy;
    mat_translate[3][2] = dz;
}
void object::set_scale(float scale_x, float scale_y, float scale_z)
{
    mat_scale[0][0] = scale_x;
    mat_scale[1][1] = scale_y;
    mat_scale[2][2] = scale_z;
}
void object::set_discretization(float d_phi, float d_theta)
{
    d1 = d_phi;
    d2 = d_theta;
}

void object::render(engine* _engine)
{
    for (uint i = 0; i < size(); i++)
        _engine->render_triangle(get_triangle(i));
}

sphere_game::sphere_game(engine* e)
{
    _engine = e;
}

void sphere_game::add_object(const object& obj)
{
    _objects.push_back(obj);
}

void sphere_game::remove_object(size_t index)
{
    assert(index < _objects.size());
    _objects.erase(_objects.begin() + index);
}

object sphere_game::get_object(size_t index)
{
    assert(index < _objects.size());
    return _objects[index];
}