#include "culinder.h"

culinder::culinder(float r, float h)
{
    radius = r;
    height = h;

    set_rotate(0, 0, 0);
    set_scale(1.f, 1.f, 1.f);
    set_translate(0, 0, 0);
    calculate();
}

vertex culinder::get_vertex(double phi, double h, rgba color)
{
    return vertex{ static_cast<float>(radius * cos(phi)),
                   static_cast<float>(radius * sin(phi)),
                   static_cast<float>(h),
                   color.r,
                   color.g,
                   color.b,
                   color.a };
}

uint32_t culinder::get_index(int phi_c, int h_c)
{
    if (h_c == 0)
    {
        return 0;
    }
    else if (h_c == static_cast<int>(height / d2) + 1)
    {
        return 1 + (static_cast<int>(height / d2)) *
                       (static_cast<int>(2 * M_PI / d1) + 1);
    }
    else
    {
        return 1 + (h_c - 1) * static_cast<int>(2 * M_PI / d1 + 1) +
               phi_c % static_cast<int>(2 * M_PI / d1 + 1);
    }
};

void culinder::calculate()
{
    int    index = 0;
    double h     = 0, phi;

    _vertexes.resize(0);
    indexes.resize(0);

    _vertexes.push_back(vertex{ 0, 0, 0, 0, 0, 0, 1. });
    for (int h_counter = 0; h_counter <= height / d2; h_counter++)
    {
        phi = 0;
        for (int phi_counter = 0; phi_counter <= 2 * M_PI / d1; phi_counter++)
        {
            if (h_counter != 0)
                _vertexes.push_back(get_vertex(
                    phi,
                    h,
                    rgba{
                        static_cast<float>(cos(M_PI * h / height)), 0, 0, 1 }));

            if (h_counter == 0)
            {
                indexes.push_back(get_index(phi_counter, h_counter));
                indexes.push_back(get_index(phi_counter, h_counter + 1));
                indexes.push_back(get_index(phi_counter + 1, h_counter + 1));
            }
            else if (h_counter < static_cast<int>(height / d2))
            {
                indexes.push_back(get_index(phi_counter + 1, h_counter));
                indexes.push_back(get_index(phi_counter, h_counter));
                indexes.push_back(get_index(phi_counter + 1, h_counter + 1));

                indexes.push_back(get_index(phi_counter, h_counter));
                indexes.push_back(get_index(phi_counter, h_counter + 1));
                indexes.push_back(get_index(phi_counter + 1, h_counter + 1));
            }
            else
            {
                indexes.push_back(get_index(phi_counter + 1, h_counter));
                indexes.push_back(get_index(phi_counter, h_counter));
                indexes.push_back(get_index(phi_counter + 1, h_counter + 1));
            }
            phi += d1;
        }
        h += d2;
    }
    _vertexes.push_back(vertex{ 0, 0, static_cast<float>(h) });
    count_triangles = indexes.size() / 3;
}
