#include "sphere.h"

sphere::sphere(float r)
{
    radius = r;

    set_rotate(0, 0, 0);
    set_scale(1.f, 1.f, 1.f);
    set_translate(0, 0, 0);
    calculate();
}

vertex sphere::get_vertex(double phi, double theta, rgba color)
{

    return vertex{ static_cast<float>(radius * (cos(phi) * sin(theta))),
                   static_cast<float>(radius * (sin(phi) * sin(theta))),
                   static_cast<float>(radius * (cos(theta))),
                   color.r,
                   color.g,
                   color.b,
                   color.a };
}

uint32_t sphere::get_index(int phi_c, int theta_c)
{
    if (theta_c == 0)
    {
        return 0;
    }
    else if (theta_c == static_cast<int>(M_PI / d2) + 1)
    {
        return 1 + (static_cast<int>(M_PI / d2)) *
                       (static_cast<int>(2 * M_PI / d1) + 1);
    }
    else
    {
        return 1 + (theta_c - 1) * static_cast<int>(2 * M_PI / d1 + 1) +
               phi_c % static_cast<int>(2 * M_PI / d1 + 1);
    }
};

void sphere::calculate()
{
    int    index = 0;
    double theta = 0, phi;

    _vertexes.resize(0);
    indexes.resize(0);

    _vertexes.push_back(get_vertex(0, 0, rgba{ 1 }));
    for (int theta_counter = 0; theta_counter <= M_PI / d2; theta_counter++)
    {
        phi = 0;
        for (int phi_counter = 0; phi_counter <= 2 * M_PI / d1; phi_counter++)
        {
            if (theta_counter != 0)
                _vertexes.push_back(get_vertex(
                    phi,
                    theta,
                    rgba{ static_cast<float>(std::pow(std::cos(theta), 2)),
                          0.,
                          0.,
                          1. }));

            if (theta_counter == 0)
            {
                indexes.push_back(get_index(phi_counter, theta_counter + 1));
                indexes.push_back(get_index(phi_counter, theta_counter));
                indexes.push_back(
                    get_index(phi_counter + 1, theta_counter + 1));
            }
            else if (theta_counter < static_cast<int>(M_PI / d2))
            {
                indexes.push_back(get_index(phi_counter, theta_counter));
                indexes.push_back(get_index(phi_counter + 1, theta_counter));
                indexes.push_back(
                    get_index(phi_counter + 1, theta_counter + 1));

                indexes.push_back(get_index(phi_counter, theta_counter + 1));
                indexes.push_back(get_index(phi_counter, theta_counter));
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
    _vertexes.push_back(get_vertex(0, M_PI, rgba{ 1 }));
    count_triangles = indexes.size() / 3;
}
