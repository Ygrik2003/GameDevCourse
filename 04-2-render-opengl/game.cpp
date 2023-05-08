#include "game.h"
#include <cassert>

#include <iostream>

sphere::sphere(float r)
{
    set_rotate(0, 0, 0);
    set_scale(1.f, 1.f, 1.f);
    set_translate(0, 0, 0);
    radius = r;
    calculate();
}

void sphere::calculate()
{
    int    index = 0;
    double theta = 0, phi;

    _vertexes.resize(0);
    indexes.resize(0);

    _vertexes.push_back(get_sphere(0, 0, radius));
    for (int theta_counter = 0; theta_counter <= M_PI / d2; theta_counter++)
    {
        phi = 0;
        for (int phi_counter = 0; phi_counter <= 2 * M_PI / d1; phi_counter++)
        {
            if (theta_counter != 0)
                _vertexes.push_back(get_sphere(phi, theta, radius));

            if (theta_counter == 0)
            {
                indexes.push_back(get_index(phi_counter, theta_counter));
                indexes.push_back(get_index(phi_counter, theta_counter + 1));
                indexes.push_back(
                    get_index(phi_counter + 1, theta_counter + 1));
            }
            else if (theta_counter < static_cast<int>(M_PI / d2))
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
    _vertexes.push_back(get_sphere(0, M_PI, radius));
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
        return (static_cast<int>(M_PI / d2)) *
                   (static_cast<int>(2 * M_PI / d1) + 1) +
               1;
    }
    else
    {
        return 1 + (theta_c - 1) * static_cast<int>(2 * M_PI / d1 + 1) +
               phi_c % static_cast<int>(2 * M_PI / d1 + 1);
    }
};
vertex sphere::get_sphere(double phi, double theta, uint r)
{

    return vertex{ static_cast<float>(r * (cos(phi) * sin(theta))),
                   static_cast<float>(r * (sin(phi) * sin(theta))),
                   static_cast<float>(r * (cos(theta))),
                   static_cast<float>(std::pow(std::cos(theta), 2)),
                   0,
                   0 };
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
    for (uint i = 0; i < size(); i++)
    {
        triangle tr = get_triangle(i);
        _engine->render_triangle(tr, tr_obj, _camera.get_transform());
    }
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

object& sphere_game::get_object(size_t index)
{
    assert(index < _objects.size());
    return _objects[index];
}

void camera::set_rotate(float alpha, float beta, float gamma)
{
    tr_cam.rotate[0][0] = cos(alpha) * cos(beta);
    tr_cam.rotate[0][1] =
        cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma);
    tr_cam.rotate[0][2] =
        cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma);

    tr_cam.rotate[1][0] = sin(alpha) * cos(beta);
    tr_cam.rotate[1][1] =
        sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma);
    tr_cam.rotate[1][2] =
        sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma);

    tr_cam.rotate[2][0] = -sin(beta);
    tr_cam.rotate[2][1] = cos(beta) * sin(gamma);
    tr_cam.rotate[2][2] = cos(beta) * cos(gamma);

    tr_cam.rotate[3][3] = 1;
}

camera::camera(float front, float back, float fovy, float aspect)
{
    set_rotate(0., 0., 0.);
    set_translate(0., 0., 0.);
    set_scale(1., 1., 1.);

    tr_cam.projection[0][0] = 1 / (aspect * std::tan(fovy / 2));
    tr_cam.projection[1][1] = 1 / std::tan(fovy / 2);
    tr_cam.projection[2][2] = (back + front) / (back - front);
    tr_cam.projection[2][3] = -2 * back * front / (back - front);
    tr_cam.projection[3][2] = 1;
}

void camera::set_translate(float dx, float dy, float dz)
{
    std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
    tr_cam.translate[3][0] = dx;
    tr_cam.translate[3][1] = dy;
    tr_cam.translate[3][2] = dz;

    tr_cam.translate[0][0] = 1;
    tr_cam.translate[1][1] = 1;
    tr_cam.translate[2][2] = 1;
    tr_cam.translate[3][3] = 1;
}

void camera::set_scale(float x, float y, float z)
{
    tr_cam.scale[0][0] = x;
    tr_cam.scale[1][1] = y;
    tr_cam.scale[2][2] = z;
    tr_cam.scale[3][3] = 1;
}

void camera::move(float dx, float dy, float dz)
{
    pos.x += dx;
    pos.y += dy;
    pos.z += dz;

    set_translate(pos.x, pos.y, pos.z);
}

transformation_camera& camera::get_transform()
{
    return tr_cam;
}
